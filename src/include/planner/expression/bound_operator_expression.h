//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_operator_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

class BoundOperatorExpression : public Expression {
 public:
  BoundOperatorExpression(ExpressionType type, LogicalType return_type);

  vector<unique_ptr<Expression>> children;

 public:
  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
