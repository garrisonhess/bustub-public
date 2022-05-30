#include "planner/operator/logical_unnest.h"

namespace bustub {

vector<ColumnBinding> LogicalUnnest::GetColumnBindings() {
  auto child_bindings = children[0]->GetColumnBindings();
  for (uint64_t i = 0; i < expressions.size(); i++) {
    child_bindings.emplace_back(unnest_index, i);
  }
  return child_bindings;
}

void LogicalUnnest::ResolveTypes() {
  types.insert(types.end(), children[0]->types.begin(), children[0]->types.end());
  for (auto &expr : expressions) {
    types.push_back(expr->return_type);
  }
}

}  // namespace bustub
