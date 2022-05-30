#include "planner/operator/logical_aggregate.h"
#include "common/string_util.h"

namespace bustub {

LogicalAggregate::LogicalAggregate(uint64_t group_index, uint64_t aggregate_index,
                                   vector<unique_ptr<Expression>> select_list)
    : LogicalOperator(LogicalOperatorType::LOGICAL_AGGREGATE_AND_GROUP_BY, move(select_list)),
      group_index(group_index),
      aggregate_index(aggregate_index),
      groupings_index(DConstants::INVALID_INDEX) {}

void LogicalAggregate::ResolveTypes() {
  D_ASSERT(groupings_index != DConstants::INVALID_INDEX || grouping_functions.empty());
  for (auto &expr : groups) {
    types.push_back(expr->return_type);
  }
  // get the chunk types from the projection list
  for (auto &expr : expressions) {
    types.push_back(expr->return_type);
  }
  for (uint64_t i = 0; i < grouping_functions.size(); i++) {
    types.emplace_back(Type::BIGINT);
  }
}

vector<ColumnBinding> LogicalAggregate::GetColumnBindings() {
  D_ASSERT(groupings_index != DConstants::INVALID_INDEX || grouping_functions.empty());
  vector<ColumnBinding> result;
  for (uint64_t i = 0; i < groups.size(); i++) {
    result.emplace_back(group_index, i);
  }
  for (uint64_t i = 0; i < expressions.size(); i++) {
    result.emplace_back(aggregate_index, i);
  }
  for (uint64_t i = 0; i < grouping_functions.size(); i++) {
    result.emplace_back(groupings_index, i);
  }
  return result;
}

string LogicalAggregate::ParamsToString() const {
  string result;
  for (uint64_t i = 0; i < groups.size(); i++) {
    if (i > 0) {
      result += "\n";
    }
    result += groups[i]->GetName();
  }
  for (uint64_t i = 0; i < expressions.size(); i++) {
    if (i > 0 || !groups.empty()) {
      result += "\n";
    }
    result += expressions[i]->GetName();
  }
  return result;
}

}  // namespace bustub
