#include "planner/expression_binder/select_binder.h"

#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

SelectBinder::SelectBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info)
    : ExpressionBinder(binder, context, false), inside_window_(false), node_(node), info_(info) {}

BindResult SelectBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
  auto &expr = **expr_ptr;
  //   // check if the expression binds to one of the groups
  //   auto group_index = TryBindGroup(expr, depth);
  //   if (group_index != INVALID_INDEX) {
  //     return BindGroup(expr, depth, group_index);
  //   }
  switch (expr.expression_class_) {
    case ExpressionClass::DEFAULT:
      return BindResult("SELECT clause cannot contain DEFAULT clause");
    case ExpressionClass::WINDOW:
      throw NotImplementedException("No window functions");
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth, false);
  }
}

// uint64_t SelectBinder::TryBindGroup(ParsedExpression &expr, uint64_t depth) {
//   // first check the group alias map, if expr is a ColumnRefExpression
//   if (expr.type == ExpressionType::COLUMN_REF) {
//     auto &colref = (ColumnRefExpression &)expr;
//     if (!colref.IsQualified()) {
//       auto alias_entry = info.alias_map.find(colref.column_names[0]);
//       if (alias_entry != info.alias_map.end()) {
//         // found entry!
//         return alias_entry->second;
//       }
//     }
//   }
//   // no alias reference found
//   // check the list of group columns for a match
//   auto entry = info.map.find(&expr);
//   if (entry != info.map.end()) {
//     return entry->second;
//   }
// #ifdef DEBUG
//   for (auto entry : info.map) {
//     assert(!entry.first->Equals(&expr));
//     assert(!expr.Equals(entry.first));
//   }
// #endif
//   return INVALID_INDEX;
// }

// BindResult SelectBinder::BindGroup(ParsedExpression &expr, uint64_t depth, uint64_t group_index) {
//   auto &group = node.groups.group_expressions[group_index];
//   return BindResult(make_unique<BoundColumnRefExpression>(expr.GetName(), group->return_type,
//                                                           ColumnBinding(node.group_index, group_index), depth));
// }

}  // namespace bustub
