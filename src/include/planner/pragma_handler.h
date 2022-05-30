//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/pragma_handler.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "parser/statement/pragma_statement.h"

namespace bustub {
class ClientContext;
class ClientContextLock;
class SQLStatement;
struct PragmaInfo;

//! Pragma handler is responsible for converting certain pragma statements into new queries
class PragmaHandler {
 public:
  explicit PragmaHandler(ClientContext &context);

  void HandlePragmaStatements(ClientContextLock &lock, vector<unique_ptr<SQLStatement>> &statements);

 private:
  ClientContext &context;

 private:
  //! Handles a pragma statement, (potentially) returning a new statement to replace the current one
  string HandlePragma(SQLStatement *statement);

  void HandlePragmaStatementsInternal(vector<unique_ptr<SQLStatement>> &statements);
};
}  // namespace bustub
