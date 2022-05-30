//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_case_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

struct BoundCaseCheck {
  unique_ptr<Expression> when_expr;
  unique_ptr<Expression> then_expr;
};

class BoundCaseExpression : public Expression {
 public:
  BoundCaseExpression(LogicalType type);
  BoundCaseExpression(unique_ptr<Expression> when_expr, unique_ptr<Expression> then_expr,
                      unique_ptr<Expression> else_expr);

  vector<BoundCaseCheck> case_checks;
  unique_ptr<Expression> else_expr;

 public:
  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
