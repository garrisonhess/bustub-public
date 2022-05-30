//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_constant_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"
#include "type/type/value.h"

namespace bustub {

class BoundConstantExpression : public Expression {
 public:
  explicit BoundConstantExpression(Value value);

  Value value;

 public:
  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;
  hash_t Hash() const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
