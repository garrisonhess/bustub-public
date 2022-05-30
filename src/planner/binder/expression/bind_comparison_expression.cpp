#include "catalog/catalog_entry/collate_catalog_entry.h"
#include "common/string_util.h"
#include "parser/expression/comparison_expression.h"
#include "planner/expression/bound_cast_expression.h"
#include "planner/expression/bound_comparison_expression.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression/bound_function_expression.h"
#include "planner/expression_binder.h"

#include "function/scalar/string_functions.h"

#include "common/types/decimal.h"

#include "catalog/catalog.h"
#include "main/config.h"

namespace bustub {

unique_ptr<Expression> ExpressionBinder::PushCollation(ClientContext &context, unique_ptr<Expression> source,
                                                       const string &collation_p, bool equality_only) {
  // replace default collation with system collation
  string collation;
  if (collation_p.empty()) {
    collation = DBConfig::GetConfig(context).collation;
  } else {
    collation = collation_p;
  }
  collation = StringUtil::Lower(collation);
  // bind the collation
  if (collation.empty() || collation == "binary" || collation == "c" || collation == "posix") {
    // binary collation: just skip
    return source;
  }
  auto &catalog = Catalog::GetCatalog(context);
  auto splits = StringUtil::Split(StringUtil::Lower(collation), ".");
  vector<CollateCatalogEntry *> entries;
  for (auto &collation_argument : splits) {
    auto collation_entry = catalog.GetEntry<CollateCatalogEntry>(context, DEFAULT_SCHEMA, collation_argument);
    if (collation_entry->combinable) {
      entries.insert(entries.begin(), collation_entry);
    } else {
      if (!entries.empty() && !entries.back()->combinable) {
        throw BinderException("Cannot combine collation types \"%s\" and \"%s\"", entries.back()->name,
                              collation_entry->name);
      }
      entries.push_back(collation_entry);
    }
  }
  for (auto &collation_entry : entries) {
    if (equality_only && collation_entry->not_required_for_equality) {
      continue;
    }
    vector<unique_ptr<Expression>> children;
    children.push_back(move(source));
    auto function = ScalarFunction::BindScalarFunction(context, collation_entry->function, move(children));
    source = move(function);
  }
  return source;
}

void ExpressionBinder::TestCollation(ClientContext &context, const string &collation) {
  PushCollation(context, make_unique<BoundConstantExpression>(Value("")), collation);
}

LogicalType BoundComparisonExpression::BindComparison(LogicalType left_type, LogicalType right_type) {
  auto result_type = LogicalType::MaxLogicalType(left_type, right_type);
  switch (result_type.id()) {
    case LogicalTypeId::DECIMAL: {
      // result is a decimal: we need the maximum width and the maximum scale over width
      vector<LogicalType> argument_types = {left_type, right_type};
      uint8_t max_width = 0, max_scale = 0, max_width_over_scale = 0;
      for (uint64_t i = 0; i < argument_types.size(); i++) {
        uint8_t width, scale;
        auto can_convert = argument_types[i].GetDecimalProperties(width, scale);
        if (!can_convert) {
          return result_type;
        }
        max_width = MaxValue<uint8_t>(width, max_width);
        max_scale = MaxValue<uint8_t>(scale, max_scale);
        max_width_over_scale = MaxValue<uint8_t>(width - scale, max_width_over_scale);
      }
      max_width = MaxValue<uint8_t>(max_scale + max_width_over_scale, max_width);
      if (max_width > Decimal::MAX_WIDTH_DECIMAL) {
        // target width does not fit in decimal: truncate the scale (if possible) to try and make it fit
        max_width = Decimal::MAX_WIDTH_DECIMAL;
      }
      return LogicalType::DECIMAL(max_width, max_scale);
    }
    case LogicalTypeId::VARCHAR:
      // for comparison with strings, we prefer to bind to the numeric types
      if (left_type.IsNumeric() || left_type.id() == LogicalTypeId::BOOLEAN) {
        return left_type;
      } else if (right_type.IsNumeric() || right_type.id() == LogicalTypeId::BOOLEAN) {
        return right_type;
      } else {
        // else: check if collations are compatible
        auto left_collation = StringType::GetCollation(left_type);
        auto right_collation = StringType::GetCollation(right_type);
        if (!left_collation.empty() && !right_collation.empty() && left_collation != right_collation) {
          throw BinderException("Cannot combine types with different collation!");
        }
      }
      return result_type;
    default:
      return result_type;
  }
}

BindResult ExpressionBinder::BindExpression(ComparisonExpression &expr, uint64_t depth) {
  // first try to bind the children of the case expression
  string error;
  BindChild(expr.left, depth, error);
  BindChild(expr.right, depth, error);
  if (!error.empty()) {
    return BindResult(error);
  }
  // the children have been successfully resolved
  auto &left = (BoundExpression &)*expr.left;
  auto &right = (BoundExpression &)*expr.right;
  auto left_sql_type = left.expr->return_type;
  auto right_sql_type = right.expr->return_type;
  // cast the input types to the same type
  // now obtain the result type of the input types
  auto input_type = BoundComparisonExpression::BindComparison(left_sql_type, right_sql_type);
  // add casts (if necessary)
  left.expr = BoundCastExpression::AddCastToType(move(left.expr), input_type, input_type.id() == LogicalTypeId::ENUM);
  right.expr = BoundCastExpression::AddCastToType(move(right.expr), input_type, input_type.id() == LogicalTypeId::ENUM);

  if (input_type.id() == LogicalTypeId::VARCHAR) {
    // handle collation
    auto collation = StringType::GetCollation(input_type);
    left.expr = PushCollation(context, move(left.expr), collation, expr.type == ExpressionType::COMPARE_EQUAL);
    right.expr = PushCollation(context, move(right.expr), collation, expr.type == ExpressionType::COMPARE_EQUAL);
  }
  // now create the bound comparison expression
  return BindResult(make_unique<BoundComparisonExpression>(expr.type, move(left.expr), move(right.expr)));
}

}  // namespace bustub
