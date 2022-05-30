//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_comparison_join.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.h"
#include "common/unordered_set.h"
#include "planner/joinside.h"
#include "planner/operator/logical_join.h"

namespace bustub {

//! LogicalComparisonJoin represents a join that involves comparisons between the LHS and RHS
class LogicalComparisonJoin : public LogicalJoin {
 public:
  explicit LogicalComparisonJoin(JoinType type,
                                 LogicalOperatorType logical_type = LogicalOperatorType::LOGICAL_COMPARISON_JOIN);

  //! The conditions of the join
  vector<JoinCondition> conditions;
  //! Used for duplicate-eliminated joins
  vector<LogicalType> delim_types;

 public:
  string ParamsToString() const override;

 public:
  static unique_ptr<LogicalOperator> CreateJoin(JoinType type, unique_ptr<LogicalOperator> left_child,
                                                unique_ptr<LogicalOperator> right_child,
                                                unordered_set<uint64_t> &left_bindings,
                                                unordered_set<uint64_t> &right_bindings,
                                                vector<unique_ptr<Expression>> &expressions);
};

}  // namespace bustub
