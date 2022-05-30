#include "planner/expression_binder/returning_binder.h"

#include "planner/expression/bound_default_expression.h"

namespace bustub {

ReturningBinder::ReturningBinder(Binder &binder, ClientContext &context) : ExpressionBinder(binder, context) {}

BindResult ReturningBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                                           bool root_expression) {
  auto &expr = **expr_ptr;
  switch (expr.GetExpressionClass()) {
    case ExpressionClass::SUBQUERY:
      return BindResult("SUBQUERY is not supported in returning statements");
    case ExpressionClass::BOUND_SUBQUERY:
      return BindResult("BOUND SUBQUERY is not supported in returning statements");
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth);
  }
}

}  // namespace bustub
