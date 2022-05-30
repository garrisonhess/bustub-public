//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bound_constraint.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "parser/constraint.h"

namespace bustub {
//! Bound equivalent of Constraint
class BoundConstraint {
 public:
  explicit BoundConstraint(ConstraintType type) : type(type){};
  virtual ~BoundConstraint() {}

  ConstraintType type;
};
}  // namespace bustub
