#include "planner/expression.h"

#include "common/exception.h"
#include "common/hash_util.h"
#include "planner/expression_iterator.h"

namespace bustub {

Expression::Expression(ExpressionType type, ExpressionClass expression_class, Type return_type)
    : BaseExpression(type, expression_class), return_type_(return_type) {}

Expression::~Expression() {}

bool Expression::IsAggregate() const {
  bool is_aggregate = false;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) { is_aggregate |= child.IsAggregate(); });
  return is_aggregate;
}

bool Expression::IsWindow() const {
  bool is_window = false;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) { is_window |= child.IsWindow(); });
  return is_window;
}

bool Expression::IsScalar() const {
  bool is_scalar = true;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) {
    if (!child.IsScalar()) {
      is_scalar = false;
    }
  });
  return is_scalar;
}

bool Expression::HasSideEffects() const {
  bool has_side_effects = false;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) {
    if (child.HasSideEffects()) {
      has_side_effects = true;
    }
  });
  return has_side_effects;
}

bool Expression::PropagatesNullValues() const {
  if (type_ == ExpressionType::OPERATOR_IS_NULL || type_ == ExpressionType::OPERATOR_IS_NOT_NULL ||
      type_ == ExpressionType::COMPARE_NOT_DISTINCT_FROM || type_ == ExpressionType::COMPARE_DISTINCT_FROM ||
      type_ == ExpressionType::CONJUNCTION_OR || type_ == ExpressionType::CONJUNCTION_AND) {
    return false;
  }
  bool propagate_null_values = true;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) {
    if (!child.PropagatesNullValues()) {
      propagate_null_values = false;
    }
  });
  return propagate_null_values;
}

bool Expression::IsFoldable() const {
  bool is_foldable = true;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) {
    if (!child.IsFoldable()) {
      is_foldable = false;
    }
  });
  return is_foldable;
}

bool Expression::HasParameter() const {
  bool has_parameter = false;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) { has_parameter |= child.HasParameter(); });
  return has_parameter;
}

bool Expression::HasSubquery() const {
  bool has_subquery = false;
  ExpressionIterator::EnumerateChildren(*this, [&](const Expression &child) { has_subquery |= child.HasSubquery(); });
  return has_subquery;
}

hash_t Expression::Hash() const {
  hash_t hash = HashUtil::Hash<uint32_t>((const unsigned int *)type_);
  hash = HashUtil::CombineHashes(hash, return_type_.Hash());
  ExpressionIterator::EnumerateChildren(
      *this, [&](const Expression &child) { hash = HashUtil::CombineHashes(child.Hash(), hash); });
  return hash;
}

}  // namespace bustub
