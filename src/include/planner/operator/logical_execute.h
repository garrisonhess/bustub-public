//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_execute.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

class LogicalExecute : public LogicalOperator {
 public:
  explicit LogicalExecute(shared_ptr<PreparedStatementData> prepared_p)
      : LogicalOperator(LogicalOperatorType::LOGICAL_EXECUTE), prepared(move(prepared_p)) {
    assert(prepared);
    types = prepared->types;
  }

  shared_ptr<PreparedStatementData> prepared;

 protected:
  void ResolveTypes() override {
    // already resolved
  }
  vector<ColumnBinding> GetColumnBindings() override {
    vector<ColumnBinding> bindings;
    for (uint64_t i = 0; i < types.size(); i++) {
      bindings.push_back(ColumnBinding(0, i));
    }
    return bindings;
  }
};
}  // namespace bustub
