//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/main/connection.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.h"
#include "main/prepared_statement.h"
#include "main/query_result.h"
#include "parser/sql_statement.h"
#include "type/value.h"

namespace bustub {
class ClientContext;
class BusTub;

//! A connection to a database. This represents a (client) connection that can
//! be used to query the database.
class Connection {
 public:
  explicit Connection(BusTub &database);
  ~Connection();

  BusTub &db_;
  shared_ptr<ClientContext> context_;

 public:
  //! Issues a query to the database and materializes the result (if necessary).
  unique_ptr<QueryResult> Query(unique_ptr<SQLStatement> statement);

  // prepared statements
  template <typename... Args>
  unique_ptr<QueryResult> Query(string query, Args... args) {
    vector<Value> values;
    return QueryParamsRecursive(query, values, args...);
  }

  //! Prepare the specified query, returning a prepared statement object
  unique_ptr<PreparedStatement> Prepare(string query);
  //! Prepare the specified statement, returning a prepared statement object
  unique_ptr<PreparedStatement> Prepare(unique_ptr<SQLStatement> statement);

  //! Extract a set of SQL statements from a specific query
  vector<unique_ptr<SQLStatement>> ExtractStatements(string query);

  void BeginTransaction();
  void Commit();
  void Rollback();
};

}  // namespace bustub
