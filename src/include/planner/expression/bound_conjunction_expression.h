//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_conjunction_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

class BoundConjunctionExpression : public Expression {
 public:
  explicit BoundConjunctionExpression(ExpressionType type);
  BoundConjunctionExpression(ExpressionType type, unique_ptr<Expression> left, unique_ptr<Expression> right);

  vector<unique_ptr<Expression>> children;

 public:
  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  bool PropagatesNullValues() const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
