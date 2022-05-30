//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_comparison_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

class BoundComparisonExpression : public Expression {
 public:
  BoundComparisonExpression(ExpressionType type, unique_ptr<Expression> left, unique_ptr<Expression> right);

  unique_ptr<Expression> left;
  unique_ptr<Expression> right;

 public:
  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;

 public:
  static Type BindComparison(Type left_type, Type right_type);
};
}  // namespace bustub
