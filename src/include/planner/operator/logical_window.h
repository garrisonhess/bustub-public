//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_window.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalWindow : public LogicalOperator {
 public:
  explicit LogicalWindow(uint64_t window_index)
      : LogicalOperator(LogicalOperatorType::LOGICAL_WINDOW), window_index(window_index) {}

  uint64_t window_index;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
