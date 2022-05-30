//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_parameter_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"
#include "type/type/value.h"

namespace bustub {

class BoundParameterExpression : public Expression {
 public:
  explicit BoundParameterExpression(uint64_t parameter_nr);

  uint64_t parameter_nr;
  Value *value;

 public:
  bool IsScalar() const override;
  bool HasParameter() const override;
  bool IsFoldable() const override;

  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;
  hash_t Hash() const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
