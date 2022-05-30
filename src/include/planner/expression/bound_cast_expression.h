//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_cast_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression.h"

namespace bustub {

class BoundCastExpression : public Expression {
 public:
  BoundCastExpression(unique_ptr<Expression> child, Type target_type, bool try_cast = false);

  //! The child type
  unique_ptr<Expression> child;
  //! Whether to use try_cast or not. try_cast converts cast failures into NULLs instead of throwing an error.
  bool try_cast;

 public:
  Type source_type() { return child->return_type; }

  //! Cast an expression to the specified SQL type if required
  static unique_ptr<Expression> AddCastToType(unique_ptr<Expression> expr, const Type &target_type,
                                              bool try_cast = false);
  //! Returns true if a cast is invertible (i.e. CAST(s -> t -> s) = s for all values of s). This is not true for e.g.
  //! boolean casts, because that can be e.g. -1 -> TRUE -> 1. This is necessary to prevent some optimizer bugs.
  static bool CastIsInvertible(const Type &source_type, const Type &target_type);

  string ToString() const override;

  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
