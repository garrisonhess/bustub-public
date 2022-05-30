//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_show.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

class LogicalShow : public LogicalOperator {
 public:
  explicit LogicalShow(unique_ptr<LogicalOperator> plan) : LogicalOperator(LogicalOperatorType::LOGICAL_SHOW) {
    children.push_back(move(plan));
  }

  vector<LogicalType> types_select;
  vector<string> aliases;

 protected:
  void ResolveTypes() override {
    types = {LogicalType::VARCHAR, LogicalType::VARCHAR, LogicalType::VARCHAR,
             LogicalType::VARCHAR, LogicalType::VARCHAR, LogicalType::VARCHAR};
  }
  vector<ColumnBinding> GetColumnBindings() override {
    return {ColumnBinding(0, 0), ColumnBinding(0, 1), ColumnBinding(0, 2),
            ColumnBinding(0, 3), ColumnBinding(0, 4), ColumnBinding(0, 5)};
  }
};
}  // namespace bustub
