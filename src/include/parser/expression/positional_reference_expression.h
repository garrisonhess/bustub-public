//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/positional_reference_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {
class PositionalReferenceExpression : public ParsedExpression {
 public:
  explicit PositionalReferenceExpression(uint64_t index);

  uint64_t index_;

 public:
  bool IsScalar() const override { return false; }

  string ToString() const override;

  static bool Equals(const PositionalReferenceExpression *a, const PositionalReferenceExpression *b);
  unique_ptr<ParsedExpression> Copy() const override;
  hash_t Hash() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};
}  // namespace bustub
