#include "planner/expression/bound_columnref_expression.h"

#include <to_string>
#include "type/type/hash.h"

namespace bustub {

BoundColumnRefExpression::BoundColumnRefExpression(string alias_p, Type type, ColumnBinding binding, uint64_t depth)
    : Expression(ExpressionType::BOUND_COLUMN_REF, ExpressionClass::BOUND_COLUMN_REF, move(type)),
      binding(binding),
      depth(depth) {
  this->alias = move(alias_p);
}

BoundColumnRefExpression::BoundColumnRefExpression(Type type, ColumnBinding binding, uint64_t depth)
    : BoundColumnRefExpression(string(), move(type), binding, depth) {}

unique_ptr<Expression> BoundColumnRefExpression::Copy() {
  return make_unique<BoundColumnRefExpression>(alias, return_type, binding, depth);
}

hash_t BoundColumnRefExpression::Hash() const {
  auto result = Expression::Hash();
  result = CombineHash(result, bustub::Hash<uint64_t>(binding.column_index));
  result = CombineHash(result, bustub::Hash<uint64_t>(binding.table_index));
  return CombineHash(result, bustub::Hash<uint64_t>(depth));
}

bool BoundColumnRefExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundColumnRefExpression *)other_p;
  return other->binding == binding && other->depth == depth;
}

string BoundColumnRefExpression::ToString() const {
  if (!alias.empty()) {
    return alias;
  }
  return "#[" + to_string(binding.table_index) + "." + to_string(binding.column_index) + "]";
}

}  // namespace bustub
