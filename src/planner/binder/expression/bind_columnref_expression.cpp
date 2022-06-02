// #include "catalog/catalog_entry/schema_catalog_entry.h"
#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/constant_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression_binder.h"
#include "planner/expression_binder/where_binder.h"
#include "type/value_factory.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(ColumnRefExpression &colref_p, uint64_t depth) {
  if (binder_.GetBindingMode() == BindingMode::EXTRACT_NAMES) {
    const auto val = ValueFactory::GetNullValueByType(TypeId::INTEGER);
    return BindResult(make_unique<BoundConstantExpression>(val));
  }

  // string error_message;
  // auto expr = QualifyColumnName(colref_p, error_message);
  // if (!expr) {
  //   return BindResult(binder_.FormatError(colref_p, error_message));
  // }
  // if (expr->type_ != ExpressionType::COLUMN_REF) {
  //   return BindExpression(&expr, depth);
  // }
  // auto &colref = (ColumnRefExpression &)*expr;
  // assert(colref.column_names_.size() == 2 || colref.column_names_.size() == 3);
  // // auto &table_name = colref.column_names_.size() == 3 ? colref.column_names_[1] : colref.column_names_[0];
  // // individual column reference
  // // resolve to either a base table or a subquery expression
  // BindResult result = binder_.bind_context_.BindColumn(colref, depth);
  // if (!result.HasError()) {
  //   BoundColumnReferenceInfo ref;
  //   ref.name_ = colref.column_names_.back();
  //   ref.query_location_ = colref.query_location_;
  //   bound_columns_.push_back(move(ref));
  // } else {
  //   result.error_ = binder_.FormatError(colref_p, result.error_);
  // }
  BindResult r;
  return r;
  // return result;
}

}  // namespace bustub
