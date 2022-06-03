//===----------------------------------------------------------------------===//
//                         BusTub
//
// main/client_context.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/statement_type.h"
#include "main/database.h"
#include "main/prepared_statement.h"
#include "parser/sql_statement.h"

namespace bustub {
class ExecutionEngine;

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

 public:
  //! Directly prepare a SQL statement
  unique_ptr<PreparedStatement> Prepare(unique_ptr<SQLStatement> statement);
};
}  // namespace bustub
