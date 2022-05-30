#include "planner/operator/logical_distinct.h"
#include "common/string_util.h"

namespace bustub {

string LogicalDistinct::ParamsToString() const {
  string result = LogicalOperator::ParamsToString();
  if (!distinct_targets.empty()) {
    result += StringUtil::Join(distinct_targets, distinct_targets.size(), "\n",
                               [](const unique_ptr<Expression> &child) { return child->GetName(); });
  }

  return result;
}

}  // namespace bustub
