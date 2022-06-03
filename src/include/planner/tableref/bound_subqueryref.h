//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/tableref/bound_subqueryref.hpp
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
      : BoundTableRef(TableReferenceType::SUBQUERY), binder_(move(binder_p)), subquery_(move(subquery)) {}

  //! The binder used to bind the subquery
  shared_ptr<Binder> binder_;
  //! The bound subquery node
  unique_ptr<BoundQueryNode> subquery_;
};
}  // namespace bustub
