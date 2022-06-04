

#pragma once

#include <memory>
#include "execution/executors/abstract_executor.h"
#include "planner/plans/abstract_plan.h"

namespace bustub {
/**
 * ExecutorFactory creates executors for arbitrary plan nodes.
 */
class PlanFactory {
 public:
  std::unique_ptr<AbstractPlanNode> CreatePlan(ClientContext &context, const SQLStatement &stmt);
};
}  // namespace bustub
