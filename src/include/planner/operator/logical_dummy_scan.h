//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_dummy_scan.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalDummyScan represents a dummy scan returning a single row
class LogicalDummyScan : public LogicalOperator {
 public:
  explicit LogicalDummyScan(uint64_t table_index)
      : LogicalOperator(LogicalOperatorType::LOGICAL_DUMMY_SCAN), table_index(table_index) {}

  uint64_t table_index;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return {ColumnBinding(table_index, 0)}; }

 protected:
  void ResolveTypes() override {
    if (types.empty()) {
      types.emplace_back(Type(TypeId::INTEGER));
    }
  }
};
}  // namespace bustub
