//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_unnest_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

//! Represents a function call that has been bound to a base function
class BoundUnnestExpression : public Expression {
 public:
  explicit BoundUnnestExpression(LogicalType return_type);

  unique_ptr<Expression> child;

 public:
  bool IsFoldable() const override;
  string ToString() const override;

  hash_t Hash() const override;
  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
