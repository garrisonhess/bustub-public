//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/lambda_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "parser/parsed_expression.h"

namespace bustub {

//! LambdaExpression represents either:
//!  1. A lambda operator that can be used for e.g. mapping an expression to a list
//!  2. An OperatorExpression with the "->" operator
//! Lambda expressions are written in the form of "capture -> expr", e.g. "x -> x + 1"
class LambdaExpression : public ParsedExpression {
 public:
  LambdaExpression(unique_ptr<ParsedExpression> lhs, unique_ptr<ParsedExpression> rhs);

  unique_ptr<ParsedExpression> lhs_;
  unique_ptr<ParsedExpression> rhs_;

 public:
  string ToString() const override;

  static bool Equals(const LambdaExpression *a, const LambdaExpression *b);
  hash_t Hash() const override;

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};

}  // namespace bustub
