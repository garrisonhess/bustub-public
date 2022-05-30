#include "planner/expression_binder/select_binder.h"

#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

SelectBinder::SelectBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info)
    : ExpressionBinder(binder, context), inside_window(false), node(node), info(info) {}

BindResult SelectBinder::BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) {
  auto &expr = **expr_ptr;
  // check if the expression binds to one of the groups
  auto group_index = TryBindGroup(expr, depth);
  if (group_index != INVALID_INDEX) {
    return BindGroup(expr, depth, group_index);
  }
  switch (expr.expression_class) {
    case ExpressionClass::DEFAULT:
      return BindResult("SELECT clause cannot contain DEFAULT clause");
    case ExpressionClass::WINDOW:
      return BindWindow((WindowExpression &)expr, depth);
    default:
      return ExpressionBinder::BindExpression(expr_ptr, depth);
  }
}

uint64_t SelectBinder::TryBindGroup(ParsedExpression &expr, uint64_t depth) {
  // first check the group alias map, if expr is a ColumnRefExpression
  if (expr.type == ExpressionType::COLUMN_REF) {
    auto &colref = (ColumnRefExpression &)expr;
    if (!colref.IsQualified()) {
      auto alias_entry = info.alias_map.find(colref.column_names[0]);
      if (alias_entry != info.alias_map.end()) {
        // found entry!
        return alias_entry->second;
      }
    }
  }
  // no alias reference found
  // check the list of group columns for a match
  auto entry = info.map.find(&expr);
  if (entry != info.map.end()) {
    return entry->second;
  }
#ifdef DEBUG
  for (auto entry : info.map) {
    assert(!entry.first->Equals(&expr));
    assert(!expr.Equals(entry.first));
  }
#endif
  return INVALID_INDEX;
}

BindResult SelectBinder::BindGroupingFunction(OperatorExpression &op, uint64_t depth) {
  if (op.children.empty()) {
    throw Exception("GROUPING requires at least one child");
  }
  if (node.groups.group_expressions.empty()) {
    return BindResult(binder.FormatError(op, "GROUPING statement cannot be used without groups"));
  }
  if (op.children.size() >= 64) {
    return BindResult(binder.FormatError(op, "GROUPING statement cannot have more than 64 groups"));
  }
  vector<uint64_t> group_indexes;
  group_indexes.reserve(op.children.size());
  for (auto &child : op.children) {
    ExpressionBinder::QualifyColumnNames(binder, child);
    auto idx = TryBindGroup(*child, depth);
    if (idx == INVALID_INDEX) {
      return BindResult(binder.FormatError(
          op, StringUtil::Format("GROUPING child \"%s\" must be a grouping column", child->GetName())));
    }
    group_indexes.push_back(idx);
  }
  auto col_idx = node.grouping_functions.size();
  node.grouping_functions.push_back(move(group_indexes));
  return BindResult(make_unique<BoundColumnRefExpression>(op.GetName(), Type::BIGINT,
                                                          ColumnBinding(node.groupings_index, col_idx), depth));
}

BindResult SelectBinder::BindGroup(ParsedExpression &expr, uint64_t depth, uint64_t group_index) {
  auto &group = node.groups.group_expressions[group_index];
  return BindResult(make_unique<BoundColumnRefExpression>(expr.GetName(), group->return_type,
                                                          ColumnBinding(node.group_index, group_index), depth));
}

}  // namespace bustub
