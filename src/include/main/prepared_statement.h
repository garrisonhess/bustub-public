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
#include "type/statement_type.h"
#include "type/type.h"
#include "type/value.h"

#include <vector>

namespace bustub {
class ClientContext;
class PreparedStatementData;
using std::vector;

//! A prepared statement
class PreparedStatement {
 public:
  //! Create a successfully prepared prepared statement object with the given name
  PreparedStatement(shared_ptr<ClientContext> context, shared_ptr<PreparedStatementData> data, string query,
                    int64_t n_param);
  //! Create a prepared statement that was not successfully prepared
  explicit PreparedStatement(string error);

  ~PreparedStatement();

 public:
  //! The client context this prepared statement belongs to
  shared_ptr<ClientContext> context_;

  //! The prepared statement data
  shared_ptr<PreparedStatementData> data_;

  //! The query that is being prepared
  string query_;

  //! Whether or not the statement was successfully prepared
  bool success_;

  //! The error message (if success = false)
  string error_;

  //! The amount of bound parameters
  int64_t n_param_;

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