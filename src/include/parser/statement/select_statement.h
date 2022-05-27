//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/select_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include "parser/parsed_expression.h"
#include "parser/query_node.h"
#include "parser/sql_statement.h"
#include "parser/tableref.h"

namespace bustub {
using std::unordered_map;

class QueryNode;

//! SelectStatement is a typical SELECT clause
class SelectStatement : public SQLStatement {
 public:
  SelectStatement() : SQLStatement(StatementType::SELECT_STATEMENT) {}

  //! The main query node
  unique_ptr<QueryNode> node;

 protected:
  SelectStatement(const SelectStatement &other);

 public:
  //! Convert the SELECT statement to a string
  string ToString() const override;
  //! Create a copy of this SelectStatement
  unique_ptr<SQLStatement> Copy() const override;
  //! Serializes a SelectStatement to a stand-alone binary blob
  void Serialize(Serializer &serializer) const;
  //! Deserializes a blob back into a SelectStatement, returns nullptr if
  //! deserialization is not possible
  static unique_ptr<SelectStatement> Deserialize(Deserializer &source);
  //! Whether or not the statements are equivalent
  bool Equals(const SQLStatement *other) const;
};
}  // namespace bustub
