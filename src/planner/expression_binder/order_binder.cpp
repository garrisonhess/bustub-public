#include "planner/expression_binder/order_binder.h"

#include "common/exception.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/constant_expression.h"
#include "parser/expression/star_expression.h"
#include "parser/query_node/select_node.h"
#include "planner/expression_binder.h"

namespace bustub {

OrderBinder::OrderBinder(vector<Binder *> binders, uint64_t projection_index,
                         case_insensitive_map_t<uint64_t> &alias_map, expression_map_t<uint64_t> &projection_map,
                         uint64_t max_count)
    : binders_(move(binders)),
      projection_index_(projection_index),
      max_count_(max_count),
      extra_list_(nullptr),
      alias_map_(alias_map),
      projection_map_(projection_map) {}

OrderBinder::OrderBinder(vector<Binder *> binders, uint64_t projection_index, SelectNode &node,
                         case_insensitive_map_t<uint64_t> &alias_map, expression_map_t<uint64_t> &projection_map)
    : binders_(move(binders)),
      projection_index_(projection_index),
      alias_map_(alias_map),
      projection_map_(projection_map) {
  this->max_count_ = node.select_list_.size();
  this->extra_list_ = &node.select_list_;
}

unique_ptr<Expression> OrderBinder::CreateProjectionReference(ParsedExpression &expr, uint64_t index) {
  string alias;
  if ((extra_list_ != nullptr) && index < extra_list_->size()) {
    alias = extra_list_->at(index)->ToString();
  } else {
    alias = expr.GetName();
  }
  return make_unique<BoundColumnRefExpression>(move(alias), Type(TypeId::INVALID),
                                               ColumnBinding(projection_index_, index));
}

unique_ptr<Expression> OrderBinder::CreateExtraReference(unique_ptr<ParsedExpression> expr) {
  auto result = CreateProjectionReference(*expr, extra_list_->size());
  extra_list_->push_back(move(expr));
  return result;
}

unique_ptr<Expression> OrderBinder::Bind(unique_ptr<ParsedExpression> expr) {
  // in the ORDER BY clause we do not bind children
  // we bind ONLY to the select list
  // if there is no matching entry in the SELECT list already, we add the expression to the SELECT list and refer the
  // new expression the new entry will then be bound later during the binding of the SELECT list we also don't do
  // type resolution here: this only happens after the SELECT list has been bound
  switch (expr->expression_class_) {
    case ExpressionClass::CONSTANT:
      // Note(GH): Just returning null when order by constant, instead of using constant as a column index.
      return nullptr;
    case ExpressionClass::COLUMN_REF: {
      // COLUMN REF expression
      // check if we can bind it to an alias in the select list
      auto &colref = (ColumnRefExpression &)*expr;
      // if there is an explicit table name we can't bind to an alias
      if (colref.IsQualified()) {
        break;
      }
      // check the alias list
      auto entry = alias_map_.find(colref.column_names_[0]);
      if (entry != alias_map_.end()) {
        // it does! point it to that entry
        return CreateProjectionReference(*expr, entry->second);
      }
      break;
    }
    case ExpressionClass::POSITIONAL_REFERENCE: {
      throw NotImplementedException("No positional ref ordering yet.");
      //   auto &posref = (PositionalReferenceExpression &)*expr;
      //   return CreateProjectionReference(*expr, posref.index - 1);
    }
    default:
      break;
  }
  // general case
  // first bind the table names of this entry
  for (auto &binder : binders_) {
    ExpressionBinder::QualifyColumnNames(*binder, expr);
  }
  // first check if the ORDER BY clause already points to an entry in the projection list
  auto entry = projection_map_.find(expr.get());
  if (entry != projection_map_.end()) {
    if (entry->second == INVALID_INDEX) {
      throw Exception("Ambiguous reference to column");
    }
    // there is a matching entry in the projection list
    // just point to that entry
    return CreateProjectionReference(*expr, entry->second);
  }
  if (extra_list_ == nullptr) {
    // no extra list specified: we cannot push an extra ORDER BY clause
    throw Exception(
        "Could not ORDER BY column X: add the expression/function to every SELECT, or move "
        "the UNION into a FROM clause.");
  }
  // otherwise we need to push the ORDER BY entry into the select list
  return CreateExtraReference(move(expr));
}

}  // namespace bustub
