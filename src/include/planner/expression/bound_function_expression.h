//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_function_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "function/scalar_function.h"
#include "planner/expression.h"

namespace bustub {
class ScalarFunctionCatalogEntry;

//! Represents a function call that has been bound to a base function
class BoundFunctionExpression : public Expression {
 public:
  BoundFunctionExpression(Type return_type, ScalarFunction bound_function, vector<unique_ptr<Expression>> arguments,
                          unique_ptr<FunctionData> bind_info, bool is_operator = false);

  // The bound function expression
  ScalarFunction function;
  //! List of child-expressions of the function
  vector<unique_ptr<Expression>> children;
  //! The bound function data (if any)
  unique_ptr<FunctionData> bind_info;
  //! Whether or not the function is an operator, only used for rendering
  bool is_operator;

 public:
  bool HasSideEffects() const override;
  bool IsFoldable() const override;
  string ToString() const override;
  bool PropagatesNullValues() const override;
  hash_t Hash() const override;
  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
  void Verify() const override;
};
}  // namespace bustub
