//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_any_join.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "planner/operator/logical_join.h"

namespace bustub {

//! LogicalAnyJoin represents a join with an arbitrary expression as JoinCondition
class LogicalAnyJoin : public LogicalJoin {
 public:
  explicit LogicalAnyJoin(JoinType type);

  //! The JoinCondition on which this join is performed
  unique_ptr<Expression> condition;

 public:
  string ParamsToString() const override;
};

}  // namespace bustub
