// #include "planner/expression_binder/constant_binder.h"

// namespace bustub {

// ConstantBinder::ConstantBinder(Binder &binder, ClientContext &context, string clause)
//     : ExpressionBinder(binder, context), clause(move(clause)) {}

// BindResult ConstantBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
//                                           bool root_expression) {
//   auto &expr = **expr_ptr;
//   switch (expr.GetExpressionClass()) {
//     case ExpressionClass::COLUMN_REF:
//       return BindResult(clause + " cannot contain column names");
//     case ExpressionClass::SUBQUERY:
//       return BindResult(clause + " cannot contain subqueries");
//     case ExpressionClass::DEFAULT:
//       return BindResult(clause + " cannot contain DEFAULT clause");
//     case ExpressionClass::WINDOW:
//       return BindResult(clause + " cannot contain window functions!");
//     default:
//       return ExpressionBinder::BindExpression(expr_ptr, depth);
//   }
// }

// string ConstantBinder::UnsupportedAggregateMessage() { return clause + "cannot contain aggregates!"; }

// }  // namespace bustub
