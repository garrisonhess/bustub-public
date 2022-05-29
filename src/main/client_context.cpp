#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "main/database.h"
#include "main/query_result.h"
#include "parser/parser.h"

#include <mutex>
#include <utility>

namespace bustub {
using std::lock_guard;
using std::make_unique;
using std::mutex;

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) {
  db_ = std::move(database);
  // transaction_ = TODO;
}

unique_ptr<PreparedStatement> ClientContext::PrepareInternal(unique_ptr<SQLStatement> statement) {
  static int prepare_count = 0;
  // now write the prepared statement data into the catalog
  string prepare_name = "____bustub_internal_prepare_" + std::to_string(prepare_count);
  prepare_count_++;

  auto statement_query = statement->query_;
  auto unbound_statement = statement->Copy();
  // auto prepared = CreatePreparedStatement(query, move(statement));
  // auto result = ExecutePreparedStatement(query, *prepared);

  // if (!result->success_) {
  //   LOG_DEBUG("prepare internal failed");
  //   throw Exception(result->error_);
  // }

  unique_ptr<PreparedStatement> result = make_unique<PreparedStatement>(shared_from_this(), statement->query_);
  result->statement_type_ = StatementType::SELECT_STATEMENT;
  result->types_ = {Type(TypeId::INTEGER)};
  result->names_ = {"column1"};
  LOG_INFO("stmt query: %s", statement_query.c_str());

  //! Create a successfully prepared prepared statement object with the given name
  return result;
}

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    return PrepareInternal(move(statement));
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

}  // namespace bustub
