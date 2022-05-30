#include "planner/bound_result_modifier.h"

namespace bustub {

BoundResultModifier::BoundResultModifier(ResultModifierType type) : type_(type) {}

BoundResultModifier::~BoundResultModifier() {}

BoundOrderByNode::BoundOrderByNode(OrderType type, OrderByNullType null_order, unique_ptr<Expression> expression)
    : type(type), null_order(null_order), expression(move(expression)) {}

BoundOrderByNode BoundOrderByNode::Copy() const { return BoundOrderByNode(type, null_order, expression->Copy()); }

string BoundOrderByNode::ToString() const {
  auto str = expression->ToString();
  switch (type) {
    case OrderType::ASCENDING:
      str += " ASC";
      break;
    case OrderType::DESCENDING:
      str += " DESC";
      break;
    default:
      break;
  }

  switch (null_order) {
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