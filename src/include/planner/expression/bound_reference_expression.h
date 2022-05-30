//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_reference_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

//! A BoundReferenceExpression represents a physical index into a DataChunk
class BoundReferenceExpression : public Expression {
 public:
  BoundReferenceExpression(string alias, LogicalType type, uint64_t index);
  BoundReferenceExpression(LogicalType type, uint64_t index);

  //! Index used to access data in the chunks
  uint64_t index;

 public:
  bool IsScalar() const override { return false; }
  bool IsFoldable() const override { return false; }

  string ToString() const override;

  hash_t Hash() const override;
  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
