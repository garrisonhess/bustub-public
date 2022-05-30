//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/query_node/bound_set_operation_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/set_operation_type.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"

namespace bustub {

//! Bound equivalent of SetOperationNode
class BoundSetOperationNode : public BoundQueryNode {
 public:
  BoundSetOperationNode() : BoundQueryNode(QueryNodeType::SET_OPERATION_NODE) {}

  //! The type of set operation
  SetOperationType setop_type = SetOperationType::NONE;
  //! The left side of the set operation
  unique_ptr<BoundQueryNode> left;
  //! The right side of the set operation
  unique_ptr<BoundQueryNode> right;

  //! Index used by the set operation
  uint64_t setop_index;
  //! The binder used by the left side of the set operation
  shared_ptr<Binder> left_binder;
  //! The binder used by the right side of the set operation
  shared_ptr<Binder> right_binder;

 public:
  uint64_t GetRootIndex() override { return setop_index; }
};

}  // namespace bustub
