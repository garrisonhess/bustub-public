//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/cast_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "type/type.h"

namespace bustub {

//! CastExpression represents a type cast from one SQL type to another SQL type
class CastExpression : public ParsedExpression {
 public:
  CastExpression(Type target, unique_ptr<ParsedExpression> child, bool try_cast = false);

  //! The child of the cast expression
  unique_ptr<ParsedExpression> child_;
  //! The type to cast to
  Type cast_type_;
  //! Whether or not this is a try_cast expression
  bool try_cast_;

 public:
  string ToString() const override;

  static bool Equals(const CastExpression *a, const CastExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE>
  static string ToString(const T &entry) {
    return (entry.try_cast_ ? "TRY_CAST(" : "CAST(") + entry.child_->ToString() + " AS " + entry.cast_type_.ToString() +
           ")";
  }
};
}  // namespace bustub
