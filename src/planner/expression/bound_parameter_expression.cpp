#include "planner/expression/bound_parameter_expression.h"
#include "common/to_string.h"
#include "common/types/hash.h"

namespace bustub {

BoundParameterExpression::BoundParameterExpression(uint64_t parameter_nr)
    : Expression(ExpressionType::VALUE_PARAMETER, ExpressionClass::BOUND_PARAMETER,
                 LogicalType(LogicalTypeId::UNKNOWN)),
      parameter_nr(parameter_nr),
      value(nullptr) {}

bool BoundParameterExpression::IsScalar() const { return true; }
bool BoundParameterExpression::HasParameter() const { return true; }
bool BoundParameterExpression::IsFoldable() const { return false; }

string BoundParameterExpression::ToString() const { return to_string(parameter_nr); }

bool BoundParameterExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundParameterExpression *)other_p;
  return parameter_nr == other->parameter_nr;
}

hash_t BoundParameterExpression::Hash() const {
  hash_t result = Expression::Hash();
  result = CombineHash(bustub::Hash(parameter_nr), result);
  return result;
}

unique_ptr<Expression> BoundParameterExpression::Copy() {
  auto result = make_unique<BoundParameterExpression>(parameter_nr);
  result->value = value;
  result->return_type = return_type;
  result->CopyProperties(*this);
  return move(result);
}

}  // namespace bustub
