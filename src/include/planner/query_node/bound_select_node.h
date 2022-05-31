//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/query_node/bound_select_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/expression_map.h"
#include "parser/group_by_node.h"
#include "planner/bound_query_node.h"
#include "planner/bound_tableref.h"
#include "planner/logical_operator.h"

namespace bustub {

class BoundGroupByNode {
 public:
  //! The total set of all group expressions
  vector<unique_ptr<Expression>> group_expressions_;
  //! The different grouping sets as they map to the group expressions
  vector<GroupingSet> grouping_sets_;
};

//! Bound equivalent of SelectNode
class BoundSelectNode : public BoundQueryNode {
 public:
  BoundSelectNode() : BoundQueryNode(QueryNodeType::SELECT_NODE) {}

  //! The original unparsed expressions. This is exported after binding, because the binding might change the
  //! expressions (e.g. when a * clause is present)
  vector<unique_ptr<ParsedExpression>> original_expressions_;

  //! The projection list
  vector<unique_ptr<Expression>> select_list_;
  //! The FROM clause
  unique_ptr<BoundTableRef> from_table_;
  //! The WHERE clause
  unique_ptr<Expression> where_clause_;
  //! list of groups
  BoundGroupByNode groups_;
  //! HAVING clause
  unique_ptr<Expression> having_;
  //! QUALIFY clause
  unique_ptr<Expression> qualify_;

  //! The amount of columns in the final result
  uint64_t column_count_;

  //! Index used by the LogicalProjection
  uint64_t projection_index_;

  //! Group index used by the LogicalAggregate (only used if HasAggregation is true)
  uint64_t group_index_;
  //! Table index for the projection child of the group op
  uint64_t group_projection_index_;
  //! Aggregate index used by the LogicalAggregate (only used if HasAggregation is true)
  uint64_t aggregate_index_;
  //! Index used for GROUPINGS column references
  uint64_t groupings_index_;
  //! Aggregate functions to compute (only used if HasAggregation is true)
  vector<unique_ptr<Expression>> aggregates_;

  //! GROUPING function calls
  vector<vector<uint64_t>> grouping_functions_;

  //! Map from aggregate function to aggregate index (used to eliminate duplicate aggregates)
  expression_map_t<uint64_t> aggregate_map_;

  //! Window index used by the LogicalWindow (only used if HasWindow is true)
  uint64_t window_index_;
  //! Window functions to compute (only used if HasWindow is true)
  vector<unique_ptr<Expression>> windows_;

  uint64_t unnest_index_;
  //! Unnest expression
  vector<unique_ptr<Expression>> unnests_;

  //! Index of pruned node
  uint64_t prune_index_;
  bool need_prune_ = false;

 public:
  uint64_t GetRootIndex() override { return need_prune_ ? prune_index_ : projection_index_; }
};
}  // namespace bustub
