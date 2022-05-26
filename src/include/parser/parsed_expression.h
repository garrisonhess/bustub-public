//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/parsed_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include "parser/base_expression.h"

// #include "duckdb/common/string_util.hpp"
// #include "duckdb/common/vector.hpp"
// #include "duckdb/parser/expression_util.hpp"
// #include "duckdb/parser/qualified_name.hpp"

namespace bustub {
using std::unique_ptr;
class Serializer;
class Deserializer;
class FieldWriter;
class FieldReader;

//!  The ParsedExpression class is a base class that can represent any expression
//!  part of a SQL statement.
/*!
 The ParsedExpression class is a base class that can represent any expression
 part of a SQL statement. This is, for example, a column reference in a SELECT
 clause, but also operators, aggregates or filters. The Expression is emitted by the parser and does not contain any
 information about bindings to the catalog or to the types. ParsedExpressions are transformed into regular Expressions
 in the Binder.
 */
class ParsedExpression : public BaseExpression {
 public:
  //! Create an Expression
  ParsedExpression(ExpressionType type, ExpressionClass expression_class) : BaseExpression(type, expression_class) {}

  //! The location in the query (if any)
  int32_t query_location_ = -1;

 public:
  bool IsAggregate() const override;
  bool IsWindow() const override;
  bool HasSubquery() const override;
  bool IsScalar() const override;
  bool HasParameter() const override;

  bool Equals(const BaseExpression *other) const override;
  hash_t Hash() const override;

  //! Create a copy of this expression
  virtual unique_ptr<ParsedExpression> Copy() const = 0;

  //! Serializes an Expression to a stand-alone binary blob
  void Serialize(Serializer &serializer) const;

  //! Serializes an Expression to a stand-alone binary blob
  virtual void Serialize(FieldWriter &writer) const = 0;

  //! Deserializes a blob back into an Expression [CAN THROW:
  //! SerializationException]
  static unique_ptr<ParsedExpression> Deserialize(Deserializer &source);

 protected:
  //! Copy base Expression properties from another expression to this one,
  //! used in Copy method
  void CopyProperties(const ParsedExpression &other) {
    type_ = other.type_;
    expression_class_ = other.expression_class_;
    alias_ = other.alias_;
  }
};

}  // namespace bustub
