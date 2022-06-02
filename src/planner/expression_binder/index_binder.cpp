#include "planner/expression_binder/index_binder.h"

namespace bustub {

IndexBinder::IndexBinder(Binder &binder, ClientContext &context) : ExpressionBinder(binder, context, false) {}

BindResult IndexBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
  auto &expr = **expr_ptr;
  switch (expr.expression_class_) {
    case ExpressionClass::WINDOW:
      return BindResult("window functions are not allowed in index expressions");
    case ExpressionClass::SUBQUERY:
      return BindResult("cannot use subquery in index expressions");
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth, false);
  }
}

}  // namespace bustub
