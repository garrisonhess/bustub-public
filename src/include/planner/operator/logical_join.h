//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_join.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/join_type.h"
#include "common/unordered_set.h"
#include "planner/logical_operator.h"
#include "storage/statistics/base_statistics.h"

namespace bustub {

//! LogicalJoin represents a join between two relations
class LogicalJoin : public LogicalOperator {
 public:
  explicit LogicalJoin(JoinType type, LogicalOperatorType logical_type = LogicalOperatorType::LOGICAL_JOIN);

  // Gets the set of table references that are reachable from this node
  static void GetTableReferences(LogicalOperator &op, unordered_set<uint64_t> &bindings);
  static void GetExpressionBindings(Expression &expr, unordered_set<uint64_t> &bindings);

  //! The type of the join (INNER, OUTER, etc...)
  JoinType join_type;
  //! Table index used to refer to the MARK column (in case of a MARK join)
  uint64_t mark_index;
  //! The columns of the LHS that are output by the join
  vector<uint64_t> left_projection_map;
  //! The columns of the RHS that are output by the join
  vector<uint64_t> right_projection_map;
  //! Join Keys statistics (optional)
  vector<unique_ptr<BaseStatistics>> join_stats;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};

}  // namespace bustub
