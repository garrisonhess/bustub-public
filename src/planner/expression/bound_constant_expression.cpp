#include "planner/expression/bound_constant_expression.h"

#include "type/type/hash.h"
#include "common/value_operations/value_operations.h"

namespace bustub {

BoundConstantExpression::BoundConstantExpression(Value value_p)
    : Expression(ExpressionType::VALUE_CONSTANT, ExpressionClass::BOUND_CONSTANT, value_p.type()),
      value(move(value_p)) {}

string BoundConstantExpression::ToString() const { return value.ToSQLString(); }

bool BoundConstantExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundConstantExpression *)other_p;
  return !ValueOperations::DistinctFrom(value, other->value);
}

hash_t BoundConstantExpression::Hash() const {
  hash_t result = Expression::Hash();
  return CombineHash(value.Hash(), result);
}

unique_ptr<Expression> BoundConstantExpression::Copy() {
  auto copy = make_unique<BoundConstantExpression>(value);
  copy->CopyProperties(*this);
  return move(copy);
}

}  // namespace bustub
