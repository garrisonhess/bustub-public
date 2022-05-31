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

  //! The amount of columns in the final result
  uint64_t column_count_;

  //! Index used by the LogicalProjection
  uint64_t projection_index_;

  //! Index of pruned node
  uint64_t prune_index_;
  bool need_prune_ = false;

 public:
  uint64_t GetRootIndex() override { return need_prune_ ? prune_index_ : projection_index_; }
};
}  // namespace bustub
