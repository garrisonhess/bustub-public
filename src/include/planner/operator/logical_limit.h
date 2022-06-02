//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_limit.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalLimit represents a LIMIT clause
class LogicalLimit : public LogicalOperator {
 public:
  LogicalLimit(int64_t limit_val, int64_t offset_val, unique_ptr<Expression> limit, unique_ptr<Expression> offset);

  //! Limit and offset values in case they are constants, used in optimizations.
  int64_t limit_val_;
  int64_t offset_val_;
  //! The maximum amount of elements to emit
  unique_ptr<Expression> limit_;
  //! The offset from the start to begin emitting elements
  unique_ptr<Expression> offset_;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
