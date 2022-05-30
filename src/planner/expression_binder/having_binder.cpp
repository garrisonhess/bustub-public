// #include "planner/expression_binder/having_binder.h"

// #include "common/string_util.h"
// #include "parser/expression/columnref_expression.h"
// #include "planner/binder.h"
// #include "planner/query_node/bound_select_node.h"

// namespace bustub {

// HavingBinder::HavingBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info,
//                            case_insensitive_map_t<uint64_t> &alias_map)
//     : SelectBinder(binder, context, node, info), column_alias_binder(node, alias_map) {
//   target_type = Type(TypeId::BOOLEAN);
// }

// BindResult HavingBinder::BindColumnRef(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
//   auto &expr = (ColumnRefExpression &)**expr_ptr;
//   auto alias_result = column_alias_binder.BindAlias(*this, expr, depth, root_expression);
//   if (!alias_result.HasError()) {
//     return alias_result;
//   }

//   return BindResult(StringUtil::Format(
//       "column %s must appear in the GROUP BY clause or be used in an aggregate function", expr.ToString()));
// }

// BindResult HavingBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
//   auto &expr = **expr_ptr;
//   // check if the expression binds to one of the groups
//   auto group_index = TryBindGroup(expr, depth);
//   if (group_index != INVALID_INDEX) {
//     return BindGroup(expr, depth, group_index);
//   }
//   switch (expr.expression_class) {
//     case ExpressionClass::WINDOW:
//       return BindResult("HAVING clause cannot contain window functions!");
//     case ExpressionClass::COLUMN_REF:
//       return BindColumnRef(expr_ptr, depth, root_expression);
//     default:
//       return bustub::SelectBinder::BindExpression(expr_ptr, depth);
//   }
// }

// }  // namespace bustub
