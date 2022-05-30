#include "planner/expression/bound_unnest_expression.h"

#include "common/string_util.h"
#include "common/types/hash.h"

namespace bustub {

BoundUnnestExpression::BoundUnnestExpression(Type return_type)
    : Expression(ExpressionType::BOUND_UNNEST, ExpressionClass::BOUND_UNNEST, move(return_type)) {}

bool BoundUnnestExpression::IsFoldable() const { return false; }

string BoundUnnestExpression::ToString() const { return "UNNEST(" + child->ToString() + ")"; }

hash_t BoundUnnestExpression::Hash() const {
  hash_t result = Expression::Hash();
  return CombineHash(result, bustub::Hash("unnest"));
}

bool BoundUnnestExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundUnnestExpression *)other_p;
  if (!Expression::Equals(child.get(), other->child.get())) {
    return false;
  }
  return true;
}

unique_ptr<Expression> BoundUnnestExpression::Copy() {
  auto copy = make_unique<BoundUnnestExpression>(return_type);
  copy->child = child->Copy();
  return move(copy);
}

}  // namespace bustub
