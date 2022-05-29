//===----------------------------------------------------------------------===//
//                         DuckDB
//
// main/prepared_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include "common/constants.h"
#include "main/query_result.h"
#include "parser/sql_statement.h"
#include "type/statement_type.h"
#include "type/type.h"
#include "type/value.h"

#include <unordered_map>
#include <vector>

namespace bustub {
class ClientContext;
using std::vector;

//! A prepared statement
class PreparedStatement {
 public:
  //! Create a successfully prepared prepared statement object with the given name
  explicit PreparedStatement(shared_ptr<ClientContext> context, string query);

  //! Create a prepared statement that was not successfully prepared
  explicit PreparedStatement(string error);

  ~PreparedStatement();

 public:
  //! The client context this prepared statement belongs to
  shared_ptr<ClientContext> context_;

  //! The query that is being prepared
  string query_;

  //! Whether or not the statement was successfully prepared
  bool success_;

  //! The error message (if success = false)
  string error_;

  StatementType statement_type_;

  //! The unbound SQL statement that was prepared
  unique_ptr<SQLStatement> unbound_statement_;

  // //! The fully prepared physical plan of the prepared statement
  // unique_ptr<PhysicalOperator> plan_;

  //! The map of parameter index to the actual value entry
  std::unordered_map<int64_t, vector<unique_ptr<Value>>> value_map_;

  //! The result names of the transaction
  vector<string> names_;

  //! The result types of the transaction
  vector<Type> types_;

  //! The statement properties
  StatementProperties properties_;

 public:
  //! Returns the number of columns in the result
  int64_t ColumnCount();

  //! Returns the statement type of the underlying prepared statement object
  StatementType GetStatementType();

  //! Returns the underlying statement properties
  StatementProperties GetStatementProperties();

  //! Returns the result SQL types of the prepared statement
  const vector<Type> &GetTypes();

  //! Returns the result names of the prepared statement
  const vector<string> &GetNames();

  //! Execute the prepared statement with the given set of values
  unique_ptr<QueryResult> Execute(vector<Value> &values);
};

}  // namespace bustub