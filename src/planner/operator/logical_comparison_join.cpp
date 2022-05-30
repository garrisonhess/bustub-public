#include "planner/operator/logical_comparison_join.h"
#include "common/string_util.h"
#include "planner/expression/bound_comparison_expression.h"

namespace bustub {

LogicalComparisonJoin::LogicalComparisonJoin(JoinType join_type, LogicalOperatorType logical_type)
    : LogicalJoin(join_type, logical_type) {}

string LogicalComparisonJoin::ParamsToString() const {
  string result = JoinTypeToString(join_type);
  for (auto &condition : conditions) {
    result += "\n";
    auto expr =
        make_unique<BoundComparisonExpression>(condition.comparison, condition.left->Copy(), condition.right->Copy());
    result += expr->ToString();
  }

  return result;
}

}  // namespace bustub
