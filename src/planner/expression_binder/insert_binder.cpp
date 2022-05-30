#include "planner/expression_binder/insert_binder.h"

#include "planner/expression/bound_default_expression.h"

namespace bustub {

InsertBinder::InsertBinder(Binder &binder, ClientContext &context) : ExpressionBinder(binder, context) {}

BindResult InsertBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
  auto &expr = **expr_ptr;
  switch (expr.GetExpressionClass()) {
    case ExpressionClass::DEFAULT:
      return BindResult("DEFAULT is not allowed here!");
    case ExpressionClass::WINDOW:
      return BindResult("INSERT statement cannot contain window functions!");
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth);
  }
}

string InsertBinder::UnsupportedAggregateMessage() { return "INSERT statement cannot contain aggregates!"; }

}  // namespace bustub
