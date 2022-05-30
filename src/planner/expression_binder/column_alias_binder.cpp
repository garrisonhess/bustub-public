// #include "planner/expression_binder/column_alias_binder.h"

// #include "common/string_util.h"
// #include "parser/expression/columnref_expression.h"
// #include "planner/query_node/bound_select_node.h"

// namespace bustub {

// ColumnAliasBinder::ColumnAliasBinder(BoundSelectNode &node, const case_insensitive_map_t<uint64_t> &alias_map)
//     : node(node), alias_map(alias_map), in_alias(false) {}

// BindResult ColumnAliasBinder::BindAlias(ExpressionBinder &enclosing_binder, ColumnRefExpression &expr, uint64_t depth,
//                                         bool root_expression) {
//   if (expr.IsQualified()) {
//     return BindResult(StringUtil::Format("Alias %s cannot be qualified.", expr.ToString()));
//   }

//   auto alias_entry = alias_map.find(expr.column_names[0]);
//   if (alias_entry == alias_map.end()) {
//     return BindResult(StringUtil::Format("Alias %s is not found.", expr.ToString()));
//   }

//   // found an alias: bind the alias expression
//   assert(!in_alias);
//   auto expression = node.original_expressions[alias_entry->second]->Copy();
//   in_alias = true;
//   auto result = enclosing_binder.BindExpression(&expression, depth, root_expression);
//   in_alias = false;
//   return result;
// }

// }  // namespace bustub