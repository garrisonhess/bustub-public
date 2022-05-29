#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "main/database.h"
#include "main/prepared_statement_data.h"
#include "main/query_result.h"
#include "parser/parser.h"
#include "type/statement_type.h"

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

unique_ptr<PreparedStatementData> ClientContext::CreatePreparedStatement(const string &query,
                                                                         unique_ptr<SQLStatement> statement) {
  StatementType statement_type = statement->type_;
  auto result = make_unique<PreparedStatementData>(statement_type);

  // Planner planner(*this);
  // planner.CreatePlan(move(statement));
  // assert(planner.plan);
  // auto plan = move(planner.plan);
  result->names_ = {"column1"};
  result->types_ = {Type(TypeId::INTEGER)};
  // result->value_map_ = move(planner.value_map);
  // result->plan_ = move(physical_plan);

  return result;
}

unique_ptr<QueryResult> ClientContext::ExecutePreparedStatement(const string &query, PreparedStatementData &statement) {
  auto result = make_unique<QueryResult>(statement.statement_type_, statement.types_, statement.names_);

  while (true) {
    static int counter = 0;
    counter++;
    if ((counter % 100) == 0) {
      return result;
    }

    std::vector<Column> columns;
    auto column1 = Column("column1", TypeId::INTEGER, nullptr);
    columns.push_back(column1);
    Schema schema = Schema(columns);
    std::vector<Value> values;
    values.emplace_back(Value(TypeId::INTEGER, counter));
    auto tup = Tuple(values, &schema);
    result->data_.push_back(tup);
  }

  result->success_ = true;
  return result;
}

unique_ptr<PreparedStatement> ClientContext::PrepareInternal(unique_ptr<SQLStatement> statement) {
  auto n_param = statement->n_param_;
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

  LOG_DEBUG("about to copy unbound statement");
  shared_ptr<PreparedStatementData> prepared_data = make_shared<PreparedStatementData>(StatementType::SELECT_STATEMENT);
  prepared_data->unbound_statement_ = move(unbound_statement);

  prepared_data->types_ = {Type(TypeId::INTEGER)};
  prepared_data->names_ = {"column1"};
  prepared_data->catalog_version_ = -1;
  LOG_INFO("stmt query: %s", statement_query.c_str());

  auto t1 = shared_from_this();

  auto result_stmt = make_unique<PreparedStatement>(t1, move(prepared_data), move(statement_query), n_param);

  //! Create a successfully prepared prepared statement object with the given name
  return result_stmt;
}

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    return PrepareInternal(move(statement));
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

unique_ptr<QueryResult> ClientContext::Query(unique_ptr<SQLStatement> statement) {
  // this->query_ = query;
  unique_ptr<QueryResult> result;
  try {
    // auto prepared = CreatePreparedStatement(query, move(statement));
    // vector<Value> bound_values;
    // result = ExecutePreparedStatement(query, *prepared, move(bound_values));
  } catch (std::exception &ex) {
    LOG_DEBUG("RUNSTATEMENT INTERNAL THREW EXCEPTION");
    result = make_unique<QueryResult>(ex.what());
  }

  if (!result->success_) {
    return result;
  }

  LOG_INFO("done with runstatement");
  return result;
}

}  // namespace bustub
