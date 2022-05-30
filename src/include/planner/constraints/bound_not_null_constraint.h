//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/constraints/bound_not_null_constraint.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_constraint.h"

namespace bustub {

class BoundNotNullConstraint : public BoundConstraint {
 public:
  explicit BoundNotNullConstraint(column_t index) : BoundConstraint(ConstraintType::NOT_NULL), index(index) {}

  //! Column index this constraint pertains to
  column_t index;
};

}  // namespace bustub
