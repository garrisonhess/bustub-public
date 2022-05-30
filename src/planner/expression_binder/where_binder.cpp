// #include "planner/expression_binder/where_binder.h"
// #include "planner/expression_binder/column_alias_binder.h"

// namespace bustub {

// WhereBinder::WhereBinder(Binder &binder, ClientContext &context, ColumnAliasBinder *column_alias_binder)
//     : ExpressionBinder(binder, context), column_alias_binder(column_alias_binder) {
//   target_type = Type(TypeId::BOOLEAN);
// }

// BindResult WhereBinder::BindColumnRef(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
//   auto &expr = (ColumnRefExpression &)**expr_ptr;
//   auto result = ExpressionBinder::BindExpression(expr_ptr, depth);
//   if (!result.HasError() || !column_alias_binder) {
//     return result;
//   }

//   BindResult alias_result = column_alias_binder->BindAlias(*this, expr, depth, root_expression);
//   // This code path cannot be exercised at thispoint. #1547 might change that.
//   if (!alias_result.HasError()) {
//     return alias_result;
//   }

//   return result;
// }

// BindResult WhereBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
//   auto &expr = **expr_ptr;
//   switch (expr.GetExpressionClass()) {
//     case ExpressionClass::DEFAULT:
//       return BindResult("WHERE clause cannot contain DEFAULT clause");
//     case ExpressionClass::WINDOW:
//       return BindResult("WHERE clause cannot contain window functions!");
//     case ExpressionClass::COLUMN_REF:
//       return BindColumnRef(expr_ptr, depth, root_expression);
//     default:
//       return ExpressionBinder::BindExpression(expr_ptr, depth);
//   }
// }

// string WhereBinder::UnsupportedAggregateMessage() { return "WHERE clause cannot contain aggregates!"; }

// }  // namespace bustub
