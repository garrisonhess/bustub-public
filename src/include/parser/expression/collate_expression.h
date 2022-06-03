//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/collate_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {

//! CollateExpression represents a COLLATE statement
class CollateExpression : public ParsedExpression {
 public:
  CollateExpression(string collation, unique_ptr<ParsedExpression> child);

  //! The child of the cast expression
  unique_ptr<ParsedExpression> child_;
  //! The collation clause
  string collation_;

 public:
  string ToString() const override;

  static bool Equals(const CollateExpression *a, const CollateExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};
}  // namespace bustub
