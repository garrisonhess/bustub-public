#include "planner/expression/bound_reference_expression.h"

#include "common/common.h"
#include "common/hash_util.h"
#include "common/serializer.h"

namespace bustub {

BoundReferenceExpression::BoundReferenceExpression(string alias, Type type, uint64_t index)
    : Expression(ExpressionType::BOUND_REF, ExpressionClass::BOUND_REF, type), index_(index) {
  this->alias_ = move(alias);
}
BoundReferenceExpression::BoundReferenceExpression(Type type, uint64_t index)
    : BoundReferenceExpression(string(), type, index) {}

string BoundReferenceExpression::ToString() const {
  if (!alias_.empty()) {
    return alias_;
  }
  return "#" + std::to_string(index_);
}

bool BoundReferenceExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundReferenceExpression *)other_p;
  return other->index_ == index_;
}

hash_t BoundReferenceExpression::Hash() const {
  return HashUtil::CombineHashes(Expression::Hash(), HashUtil::Hash<uint64_t>(&index_));
}

unique_ptr<Expression> BoundReferenceExpression::Copy() {
  return make_unique<BoundReferenceExpression>(alias_, return_type_, index_);
}

}  // namespace bustub
