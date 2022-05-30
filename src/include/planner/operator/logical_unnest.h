//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_unnest.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalUnnest : public LogicalOperator {
 public:
  explicit LogicalUnnest(uint64_t unnest_index)
      : LogicalOperator(LogicalOperatorType::LOGICAL_UNNEST), unnest_index(unnest_index) {}

  uint64_t unnest_index;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
