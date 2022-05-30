//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_aggregate.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/group_by_node.h"
#include "planner/column_binding.h"
#include "planner/logical_operator.h"
#include "storage/statistics/base_statistics.h"

namespace bustub {

//! LogicalAggregate represents an aggregate operation with (optional) GROUP BY
//! operator.
class LogicalAggregate : public LogicalOperator {
 public:
  LogicalAggregate(uint64_t group_index, uint64_t aggregate_index, vector<unique_ptr<Expression>> select_list);

  //! The table index for the groups of the LogicalAggregate
  uint64_t group_index;
  //! The table index for the aggregates of the LogicalAggregate
  uint64_t aggregate_index;
  //! The table index for the GROUPING function calls of the LogicalAggregate
  uint64_t groupings_index;
  //! The set of groups (optional).
  vector<unique_ptr<Expression>> groups;
  //! The set of grouping sets (optional).
  vector<GroupingSet> grouping_sets;
  //! The list of grouping function calls (optional)
  vector<vector<uint64_t>> grouping_functions;
  //! Group statistics (optional)
  vector<unique_ptr<BaseStatistics>> group_stats;

 public:
  string ParamsToString() const override;

  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
