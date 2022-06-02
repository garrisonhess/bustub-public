#include "planner/expression/bound_constant_expression.h"

// #include "common/value_operations/value_operations.h"
#include "common/exception.h"
#include "common/hash_util.h"

namespace bustub {

BoundConstantExpression::BoundConstantExpression(const Value &value_p)
    : Expression(ExpressionType::VALUE_CONSTANT, ExpressionClass::BOUND_CONSTANT, Type(value_p.GetTypeId())),
      value_(value_p) {}

string BoundConstantExpression::ToString() const { return value_.ToString(); }

bool BoundConstantExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  // auto other = (BoundConstantExpression *)other_p;
  // return !ValueOperations::DistinctFrom(value_, other->value_);
  return true;
}

hash_t BoundConstantExpression::Hash() const {
  hash_t result = Expression::Hash();
  return HashUtil::CombineHashes(HashUtil::HashValue(&value_), result);
}

unique_ptr<Expression> BoundConstantExpression::Copy() {
  auto copy = make_unique<BoundConstantExpression>(value_);
  copy->CopyProperties(*this);
  return copy;
}

}  // namespace bustub
