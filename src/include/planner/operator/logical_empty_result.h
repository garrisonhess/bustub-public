//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_empty_result.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalEmptyResult returns an empty result. This is created by the optimizer if it can reason that ceratin parts of
//! the tree will always return an empty result.
class LogicalEmptyResult : public LogicalOperator {
 public:
  explicit LogicalEmptyResult(unique_ptr<LogicalOperator> op);

  //! The set of return types of the empty result
  vector<Type> return_types_;
  //! The columns that would be bound at this location (if the subtree was not optimized away)
  vector<ColumnBinding> bindings_;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return bindings_; }

 protected:
  void ResolveTypes() override { this->types_ = return_types_; }
};
}  // namespace bustub
