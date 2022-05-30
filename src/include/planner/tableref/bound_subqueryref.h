//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_subqueryref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/binder.h"
#include "planner/bound_query_node.h"
#include "planner/bound_tableref.h"

namespace bustub {

//! Represents a cross product
class BoundSubqueryRef : public BoundTableRef {
 public:
  BoundSubqueryRef(shared_ptr<Binder> binder_p, unique_ptr<BoundQueryNode> subquery)
      : BoundTableRef(TableReferenceType::SUBQUERY), binder(move(binder_p)), subquery(move(subquery)) {}

  //! The binder used to bind the subquery
  shared_ptr<Binder> binder;
  //! The bound subquery node
  unique_ptr<BoundQueryNode> subquery;
};
}  // namespace bustub
