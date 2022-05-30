#include "planner/expression/bound_subquery_expression.h"

#include "common/exception.h"

namespace bustub {

BoundSubqueryExpression::BoundSubqueryExpression(Type return_type)
    : Expression(ExpressionType::SUBQUERY, ExpressionClass::BOUND_SUBQUERY, move(return_type)) {}

string BoundSubqueryExpression::ToString() const { return "SUBQUERY"; }

bool BoundSubqueryExpression::Equals(const BaseExpression *other_p) const {
  // equality between bound subqueries not implemented currently
  return false;
}

unique_ptr<Expression> BoundSubqueryExpression::Copy() {
  throw SerializationException("Cannot copy BoundSubqueryExpression");
}

bool BoundSubqueryExpression::PropagatesNullValues() const {
  // TODO this can be optimized further by checking the actual subquery node
  return false;
}

}  // namespace bustub
