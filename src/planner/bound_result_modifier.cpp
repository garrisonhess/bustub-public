#include "planner/bound_result_modifier.h"

namespace bustub {

BoundResultModifier::BoundResultModifier(ResultModifierType type) : type_(type) {}

BoundResultModifier::~BoundResultModifier() = default;

BoundOrderByNode::BoundOrderByNode(OrderType type, OrderByNullType null_order, unique_ptr<Expression> expression)
    : type_(type), null_order_(null_order), expression_(move(expression)) {}

BoundOrderByNode BoundOrderByNode::Copy() const { return BoundOrderByNode(type_, null_order_, expression_->Copy()); }

string BoundOrderByNode::ToString() const {
  auto str = expression_->ToString();
  switch (type_) {
    case OrderType::ASCENDING:
      str += " ASC";
      break;
    case OrderType::DESCENDING:
      str += " DESC";
      break;
    default:
      break;
  }

  switch (null_order_) {
    case OrderByNullType::NULLS_FIRST:
      str += " NULLS FIRST";
      break;
    case OrderByNullType::NULLS_LAST:
      str += " NULLS LAST";
      break;
    default:
      break;
  }
  return str;
}

BoundLimitModifier::BoundLimitModifier() : BoundResultModifier(ResultModifierType::LIMIT_MODIFIER) {}

BoundOrderModifier::BoundOrderModifier() : BoundResultModifier(ResultModifierType::ORDER_MODIFIER) {}

BoundDistinctModifier::BoundDistinctModifier() : BoundResultModifier(ResultModifierType::DISTINCT_MODIFIER) {}

}  // namespace bustub
