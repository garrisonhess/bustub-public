//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_distinct.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalDistinct filters duplicate entries from its child operator
class LogicalDistinct : public LogicalOperator {
 public:
  LogicalDistinct() : LogicalOperator(LogicalOperatorType::LOGICAL_DISTINCT) {}
  explicit LogicalDistinct(vector<unique_ptr<Expression>> targets)
      : LogicalOperator(LogicalOperatorType::LOGICAL_DISTINCT), distinct_targets(move(targets)) {}
  //! The set of distinct targets (optional).
  vector<unique_ptr<Expression>> distinct_targets;

 public:
  string ParamsToString() const override;

  vector<ColumnBinding> GetColumnBindings() override { return children[0]->GetColumnBindings(); }

 protected:
  void ResolveTypes() override { types = children[0]->types; }
};
}  // namespace bustub
