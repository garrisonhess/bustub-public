//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/subquery_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/subquery_type.h"
#include "parser/parsed_expression.h"
#include "parser/statement/select_statement.h"

namespace bustub {

//! Represents a subquery
class SubqueryExpression : public ParsedExpression {
 public:
  SubqueryExpression();

  //! The actual subquery
  unique_ptr<SelectStatement> subquery_;
  //! The subquery type
  SubqueryType subquery_type_;
  //! the child expression to compare with (in case of IN, ANY, ALL operators, empty for EXISTS queries and scalar
  //! subquery)
  unique_ptr<ParsedExpression> child_;
  //! The comparison type of the child expression with the subquery (in case of ANY, ALL operators), empty otherwise
  ExpressionType comparison_type_;

 public:
  bool HasSubquery() const override { return true; }
  bool IsScalar() const override { return false; }

  string ToString() const override;

  static bool Equals(const SubqueryExpression *a, const SubqueryExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};
}  // namespace bustub
