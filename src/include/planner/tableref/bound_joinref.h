//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_joinref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/join_type.h"
#include "planner/binder.h"
#include "planner/bound_tableref.h"
#include "planner/expression.h"

namespace bustub {

//! Represents a join
class BoundJoinRef : public BoundTableRef {
 public:
  BoundJoinRef() : BoundTableRef(TableReferenceType::JOIN) {}

  //! The binder used to bind the LHS of the join
  shared_ptr<Binder> left_binder_;
  //! The binder used to bind the RHS of the join
  shared_ptr<Binder> right_binder_;
  //! The left hand side of the join
  unique_ptr<BoundTableRef> left_;
  //! The right hand side of the join
  unique_ptr<BoundTableRef> right_;
  //! The join condition
  unique_ptr<Expression> condition_;
  //! The join type
  JoinType type_;
};
}  // namespace bustub
