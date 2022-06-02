#include "planner/expression/bound_columnref_expression.h"

#include "common/common.h"
#include "common/hash_util.h"

namespace bustub {
using std::to_string;

BoundColumnRefExpression::BoundColumnRefExpression(string alias_p, Type type, ColumnBinding binding, uint64_t depth)
    : Expression(ExpressionType::BOUND_COLUMN_REF, ExpressionClass::BOUND_COLUMN_REF, move(type)),
      binding_(binding),
      depth_(depth) {
  this->alias_ = move(alias_p);
}

BoundColumnRefExpression::BoundColumnRefExpression(Type type, ColumnBinding binding, uint64_t depth)
    : BoundColumnRefExpression(string(), type, binding, depth) {}

unique_ptr<Expression> BoundColumnRefExpression::Copy() {
  return make_unique<BoundColumnRefExpression>(alias_, return_type_, binding_, depth_);
}

hash_t BoundColumnRefExpression::Hash() const {
  auto result = Expression::Hash();
  result = HashUtil::CombineHashes(result, HashUtil::Hash<uint64_t>(&binding_.column_index_));
  result = HashUtil::CombineHashes(result, HashUtil::Hash<uint64_t>(&binding_.table_index_));
  return HashUtil::CombineHashes(result, HashUtil::Hash<uint64_t>(&depth_));
}

bool BoundColumnRefExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundColumnRefExpression *)other_p;
  return other->binding_ == binding_ && other->depth_ == depth_;
}

string BoundColumnRefExpression::ToString() const {
  if (!alias_.empty()) {
    return alias_;
  }
  return "#[" + to_string(binding_.table_index_) + "." + to_string(binding_.column_index_) + "]";
}

}  // namespace bustub
