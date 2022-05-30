//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/constraints/bound_check_constraint.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_set>
#include "planner/bound_constraint.h"
#include "planner/expression.h"

namespace bustub {

//! The CheckConstraint contains an expression that must evaluate to TRUE for
//! every row in a table
class BoundCheckConstraint : public BoundConstraint {
 public:
  BoundCheckConstraint() : BoundConstraint(ConstraintType::CHECK) {}

  //! The expression
  unique_ptr<Expression> expression;
  //! The columns used by the CHECK constraint
  unordered_set<column_t> bound_columns;
};

}  // namespace bustub
