#include "planner/expression/bound_operator_expression.h"
#include "common/string_util.h"
#include "parser/expression/operator_expression.h"
#include "parser/expression_util.h"

namespace bustub {

BoundOperatorExpression::BoundOperatorExpression(ExpressionType type, Type return_type)
    : Expression(type, ExpressionClass::BOUND_OPERATOR, move(return_type)) {}

string BoundOperatorExpression::ToString() const {
  return OperatorExpression::ToString<BoundOperatorExpression, Expression>(*this);
}

bool BoundOperatorExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundOperatorExpression *)other_p;
  if (!ExpressionUtil::ListEquals(children, other->children)) {
    return false;
  }
  return true;
}

unique_ptr<Expression> BoundOperatorExpression::Copy() {
  auto copy = make_unique<BoundOperatorExpression>(type, return_type);
  copy->CopyProperties(*this);
  for (auto &child : children) {
    copy->children.push_back(child->Copy());
  }
  return move(copy);
}

}  // namespace bustub
