#include "planner/operator/logical_any_join.h"

namespace bustub {

LogicalAnyJoin::LogicalAnyJoin(JoinType type) : LogicalJoin(type, LogicalOperatorType::LOGICAL_ANY_JOIN) {}

string LogicalAnyJoin::ParamsToString() const { return condition->ToString(); }

}  // namespace bustub
