//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_subquery_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/subquery_type.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"
#include "planner/expression.h"

namespace bustub {

class BoundSubqueryExpression : public Expression {
 public:
  explicit BoundSubqueryExpression(Type return_type);

  bool IsCorrelated() { return binder->correlated_columns.size() > 0; }

  //! The binder used to bind the subquery node
  shared_ptr<Binder> binder;
  //! The bound subquery node
  unique_ptr<BoundQueryNode> subquery;
  //! The subquery type
  SubqueryType subquery_type;
  //! the child expression to compare with (in case of IN, ANY, ALL operators)
  unique_ptr<Expression> child;
  //! The comparison type of the child expression with the subquery (in case of ANY, ALL operators)
  ExpressionType comparison_type;
  //! The Type of the subquery result. Only used for ANY expressions.
  Type child_type;
  //! The target Type of the subquery result (i.e. to which type it should be casted, if child_type <>
  //! child_target). Only used for ANY expressions.
  Type child_target;

 public:
  bool HasSubquery() const override { return true; }
  bool IsScalar() const override { return false; }
  bool IsFoldable() const override { return false; }

  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;

  bool PropagatesNullValues() const override;
};
}  // namespace bustub
