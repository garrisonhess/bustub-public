//===----------------------------------------------------------------------===//
//                         BusTub
//
// main/client_context.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "execution/execution_engine.h"
#include "main/database.h"
#include "main/prepared_statement.h"
#include "parser/sql_statement.h"
#include "type/statement_type.h"

#include <memory>
#include <mutex>
#include <random>
#include <unordered_set>

namespace bustub {
class Catalog;
class BusTub;

//! The ClientContext holds information relevant to the current client session
//! during execution
class ClientContext : public std::enable_shared_from_this<ClientContext> {
 public:
  explicit ClientContext(shared_ptr<DatabaseInstance> database);
  ~ClientContext() = default;

  //! The database that this client is connected to
  shared_ptr<DatabaseInstance> db_;

  //! The current query being executed by the client context
  string query_;

  //! The query executor (has transaction manager and catalog)
  ExecutionEngine *executor_;

 public:
  //! Issue a query, returning a QueryResult.
  unique_ptr<QueryResult> Query(unique_ptr<SQLStatement> statement);

  //! Directly prepare a SQL statement
  unique_ptr<PreparedStatement> Prepare(unique_ptr<SQLStatement> statement);

  //! Execute a prepared statement with the given name and set of parameters
  unique_ptr<QueryResult> Execute(string name, vector<Value> &values, string query = string());

 private:
  //! Internally prepare a SQL statement. Caller must hold the context_lock.
  unique_ptr<PreparedStatement> CreatePreparedStatement(const string &query, unique_ptr<SQLStatement> statement);

  //! Internally execute a prepared SQL statement. Caller must hold the context_lock.
  unique_ptr<QueryResult> ExecutePreparedStatement(const string &query, PreparedStatement &statement);

  unique_ptr<PreparedStatement> PrepareInternal(unique_ptr<SQLStatement> statement);

 private:
  int64_t prepare_count_ = 0;

  //! Prepared statement objects that were created using the ClientContext::Prepare method
  std::unordered_set<PreparedStatement *> prepared_statement_objects_;
};
}  // namespace bustub
