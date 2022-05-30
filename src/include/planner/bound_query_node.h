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
  explicit BoundQueryNode(QueryNodeType type) : type(type) {}
  virtual ~BoundQueryNode() {}

  //! The type of the query node, either SetOperation or Select
  QueryNodeType type;
  //! The result modifiers that should be applied to this query node
  vector<unique_ptr<BoundResultModifier>> modifiers;

  //! The names returned by this QueryNode.
  vector<string> names;
  //! The types returned by this QueryNode.
  vector<Type> types;

 public:
  virtual uint64_t GetRootIndex() = 0;
};

}  // namespace bustub
