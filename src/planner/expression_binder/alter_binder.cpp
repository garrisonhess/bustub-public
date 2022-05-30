#include "planner/expression_binder/alter_binder.h"

#include "catalog/catalog_entry/table_catalog_entry.h"
#include "parser/expression/columnref_expression.h"
#include "planner/expression/bound_reference_expression.h"

namespace bustub {

AlterBinder::AlterBinder(Binder &binder, ClientContext &context, TableCatalogEntry &table,
                         vector<column_t> &bound_columns, LogicalType target_type)
    : ExpressionBinder(binder, context), table(table), bound_columns(bound_columns) {
  this->target_type = move(target_type);
}

BindResult AlterBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
  auto &expr = **expr_ptr;
  switch (expr.GetExpressionClass()) {
    case ExpressionClass::WINDOW:
      return BindResult("window functions are not allowed in alter statement");
    case ExpressionClass::SUBQUERY:
      return BindResult("cannot use subquery in alter statement");
    case ExpressionClass::COLUMN_REF:
      return BindColumn((ColumnRefExpression &)expr);
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth);
  }
}

string AlterBinder::UnsupportedAggregateMessage() { return "aggregate functions are not allowed in alter statement"; }

BindResult AlterBinder::BindColumn(ColumnRefExpression &colref) {
  if (colref.column_names.size() > 1) {
    return BindQualifiedColumnName(colref, table.name);
  }
  auto idx = table.GetColumnIndex(colref.column_names[0], true);
  if (idx == DConstants::INVALID_INDEX) {
    throw BinderException("Table does not contain column %s referenced in alter statement!", colref.column_names[0]);
  }
  bound_columns.push_back(idx);
  return BindResult(make_unique<BoundReferenceExpression>(table.columns[idx].type, bound_columns.size() - 1));
}

}  // namespace bustub
