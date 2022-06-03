//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/parameter_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {
class ParameterExpression : public ParsedExpression {
 public:
  ParameterExpression();

  uint64_t parameter_nr_;

 public:
  bool IsScalar() const override { return true; }
  bool HasParameter() const override { return true; }

  string ToString() const override;

  unique_ptr<ParsedExpression> Copy() const override;
  hash_t Hash() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};
}  // namespace bustub
