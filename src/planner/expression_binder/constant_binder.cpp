#include "planner/expression_binder/constant_binder.h"

namespace bustub {

ConstantBinder::ConstantBinder(Binder &binder, ClientContext &context, string clause)
    : ExpressionBinder(binder, context, false), clause_(move(clause)) {}

BindResult ConstantBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                                          bool root_expression) {
  auto &expr = **expr_ptr;
  switch (expr.GetExpressionClass()) {
    case ExpressionClass::COLUMN_REF:
      return BindResult(clause_ + " cannot contain column names");
    case ExpressionClass::SUBQUERY:
      return BindResult(clause_ + " cannot contain subqueries");
    case ExpressionClass::DEFAULT:
      return BindResult(clause_ + " cannot contain DEFAULT clause");
    case ExpressionClass::WINDOW:
      return BindResult(clause_ + " cannot contain window functions!");
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth, false);
  }
}

}  // namespace bustub
