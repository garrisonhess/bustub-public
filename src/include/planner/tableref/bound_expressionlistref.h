//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_expressionlistref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_tableref.h"
#include "planner/expression.h"

namespace bustub {
//! Represents a TableReference to a base table in the schema
class BoundExpressionListRef : public BoundTableRef {
 public:
  BoundExpressionListRef() : BoundTableRef(TableReferenceType::EXPRESSION_LIST) {}

  //! The bound VALUES list
  vector<vector<unique_ptr<Expression>>> values_;
  //! The generated names of the values list
  vector<string> names_;
  //! The types of the values list
  vector<Type> types_;
  //! The index in the bind context
  uint64_t bind_index_;
};
}  // namespace bustub
