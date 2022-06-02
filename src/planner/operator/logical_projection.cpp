#include "planner/operator/logical_projection.h"

namespace bustub {

LogicalProjection::LogicalProjection(uint64_t table_index, vector<unique_ptr<Expression>> select_list)
    : LogicalOperator(LogicalOperatorType::LOGICAL_PROJECTION, move(select_list)), table_index_(table_index) {}

vector<ColumnBinding> LogicalProjection::GetColumnBindings() {
  return GenerateColumnBindings(table_index_, expressions_.size());
}

void LogicalProjection::ResolveTypes() {
  for (auto &expr : expressions_) {
    types_.push_back(expr->return_type_);
  }
}

}  // namespace bustub
