//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_default_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

class BoundDefaultExpression : public Expression {
 public:
  explicit BoundDefaultExpression(Type type = Type())
      : Expression(ExpressionType::VALUE_DEFAULT, ExpressionClass::BOUND_DEFAULT, type) {}

 public:
  bool IsScalar() const override { return false; }
  bool IsFoldable() const override { return false; }

  string ToString() const override { return "DEFAULT"; }

  unique_ptr<Expression> Copy() override { return make_unique<BoundDefaultExpression>(return_type); }
};
}  // namespace bustub
