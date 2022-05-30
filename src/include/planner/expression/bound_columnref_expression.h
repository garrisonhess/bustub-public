//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_columnref_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/column_binding.h"
#include "planner/expression.h"

namespace bustub {

//! A BoundColumnRef expression represents a ColumnRef expression that was bound to an actual table and column index. It
//! is not yet executable, however. The ColumnBindingResolver transforms the BoundColumnRefExpressions into
//! BoundExpressions, which refer to indexes into the physical chunks that pass through the executor.
class BoundColumnRefExpression : public Expression {
 public:
  BoundColumnRefExpression(LogicalType type, ColumnBinding binding, uint64_t depth = 0);
  BoundColumnRefExpression(string alias, LogicalType type, ColumnBinding binding, uint64_t depth = 0);

  //! Column index set by the binder, used to generate the final BoundExpression
  ColumnBinding binding;
  //! The subquery depth (i.e. depth 0 = current query, depth 1 = parent query, depth 2 = parent of parent, etc...).
  //! This is only non-zero for correlated expressions inside subqueries.
  uint64_t depth;

 public:
  bool IsScalar() const override { return false; }
  bool IsFoldable() const override { return false; }

  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;
  hash_t Hash() const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
