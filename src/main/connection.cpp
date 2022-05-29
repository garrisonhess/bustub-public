#include "main/connection.h"

#include "main/client_context.h"
#include "main/connection_manager.h"
#include "main/database.h"
#include "parser/parser.h"

namespace bustub {

Connection::Connection(BusTub &database)
    : db_(database), context_(std::make_shared<ClientContext>(database.instance_)) {}

Connection::~Connection() = default;

unique_ptr<QueryResult> Connection::Query(unique_ptr<SQLStatement> statement) {
  return context_->Query(move(statement));
}

unique_ptr<PreparedStatement> Connection::Prepare(unique_ptr<SQLStatement> statement) {
  return context_->Prepare(move(statement));
}

void Connection::BeginTransaction() {
  // auto result = Query("BEGIN TRANSACTION");
  // if (!result->success_) {
  // 	throw Exception("err");
  // }
}

void Connection::Commit() {
  // auto result = Query("COMMIT");
  // if (!result->success) {
  // 	throw Exception(result->error);
  // }
}

void Connection::Rollback() {
  // auto result = Query("ROLLBACK");
  // if (!result->success) {
  // 	throw Exception(result->error);
  // }
}

}  // namespace bustub
