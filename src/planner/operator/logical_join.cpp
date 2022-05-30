#include "planner/operator/logical_join.h"

#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression_iterator.h"

namespace bustub {

LogicalJoin::LogicalJoin(JoinType join_type, LogicalOperatorType logical_type)
    : LogicalOperator(logical_type), join_type(join_type) {}

vector<ColumnBinding> LogicalJoin::GetColumnBindings() {
  auto left_bindings = MapBindings(children[0]->GetColumnBindings(), left_projection_map);
  // for other join types we project both the LHS and the RHS
  auto right_bindings = MapBindings(children[1]->GetColumnBindings(), right_projection_map);
  left_bindings.insert(left_bindings.end(), right_bindings.begin(), right_bindings.end());
  return left_bindings;
}

void LogicalJoin::ResolveTypes() {
  types = MapTypes(children[0]->types, left_projection_map);
  // for any other join we project both sides
  auto right_types = MapTypes(children[1]->types, right_projection_map);
  types.insert(types.end(), right_types.begin(), right_types.end());
}

void LogicalJoin::GetTableReferences(LogicalOperator &op, unordered_set<uint64_t> &bindings) {
  auto column_bindings = op.GetColumnBindings();
  for (auto binding : column_bindings) {
    bindings.insert(binding.table_index);
  }
}

void LogicalJoin::GetExpressionBindings(Expression &expr, unordered_set<uint64_t> &bindings) {
  if (expr.type == ExpressionType::BOUND_COLUMN_REF) {
    auto &colref = (BoundColumnRefExpression &)expr;
    assert(colref.depth == 0);
    bindings.insert(colref.binding.table_index);
  }
  ExpressionIterator::EnumerateChildren(expr, [&](Expression &child) { GetExpressionBindings(child, bindings); });
}

}  // namespace bustub