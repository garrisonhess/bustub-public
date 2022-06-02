#include "planner/operator/logical_distinct.h"
#include "common/string_util.h"

namespace bustub {
using std::string;

string LogicalDistinct::ParamsToString() const {
  string result = LogicalOperator::ParamsToString();
  if (!distinct_targets_.empty()) {
    result += StringUtil::Join(distinct_targets_, distinct_targets_.size(), "\n",
                               [](const unique_ptr<Expression> &child) { return child->GetName(); });
  }

  return result;
}

}  // namespace bustub
