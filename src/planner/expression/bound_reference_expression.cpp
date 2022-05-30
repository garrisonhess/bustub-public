#include "planner/expression/bound_reference_expression.h"

#include "common/serializer.h"
#include "common/hash_util.h"
#include <to_string>

namespace bustub {

BoundReferenceExpression::BoundReferenceExpression(string alias, Type type, uint64_t index)
    : Expression(ExpressionType::BOUND_REF, ExpressionClass::BOUND_REF, move(type)), index(index) {
  this->alias = move(alias);
}
BoundReferenceExpression::BoundReferenceExpression(Type type, uint64_t index)
    : BoundReferenceExpression(string(), move(type), index) {}

string BoundReferenceExpression::ToString() const {
  if (!alias.empty()) {
    return alias;
  }
  return "#" + to_string(index);
}

bool BoundReferenceExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundReferenceExpression *)other_p;
  return other->index == index;
}

hash_t BoundReferenceExpression::Hash() const { return CombineHash(Expression::Hash(), bustub::Hash<uint64_t>(index)); }

unique_ptr<Expression> BoundReferenceExpression::Copy() {
  return make_unique<BoundReferenceExpression>(alias, return_type, index);
}

}  // namespace bustub
