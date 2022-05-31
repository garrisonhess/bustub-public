//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bound_query_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/query_node.h"
#include "planner/bound_result_modifier.h"
#include "planner/expression.h"

namespace bustub {

//! Bound equivalent of QueryNode
class BoundQueryNode {
 public:
  explicit BoundQueryNode(QueryNodeType type) : type_(type) {}
  virtual ~BoundQueryNode() = default;

  //! The type of the query node, either SetOperation or Select
  QueryNodeType type_;
  //! The result modifiers that should be applied to this query node
  vector<unique_ptr<BoundResultModifier>> modifiers_;

  //! The names returned by this QueryNode.
  vector<string> names_;
  //! The types returned by this QueryNode.
  vector<Type> types_;

 public:
  virtual uint64_t GetRootIndex() = 0;
};

}  // namespace bustub
