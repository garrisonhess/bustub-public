

#pragma once

#include <memory>
#include "execution/executors/abstract_executor.h"
#include "planner/plans/abstract_plan.h"

namespace bustub {

class PlanFactory {
 public:
  std::unique_ptr<AbstractPlanNode> CreatePlan(ClientContext &context, SQLStatement &stmt);
};

}  // namespace bustub
