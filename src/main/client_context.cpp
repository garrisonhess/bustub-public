#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "main/database.h"
#include "main/query_result.h"
#include "parser/parser.h"
#include "planner/planner.h"

#include <mutex>
#include <utility>

namespace bustub {

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) { db_ = std::move(database); }

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    LOG_INFO("preparing statement: %s", statement->ToString().c_str());
    static int prepare_count = 0;
    string prepare_name = "____bustub_internal_prepare_" + std::to_string(prepare_count);
    prepare_count_++;

    LOG_INFO("About to call planner w/ query: %s", statement->query_.c_str());
    Planner planner = Planner(*this);
    planner.CreatePlan(move(statement));
    LOG_INFO("Done w/ CreatePlan.");
    LOG_INFO("planner plan: %s", planner.plan_->ToString().c_str());

    // planner.plan_
    // planner.binder_
    // planner.context_
    // planner.names_
    // planner.types_

    unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(), statement->query_);
    result->statement_type_ = StatementType::SELECT_STATEMENT;
    result->types_ = {Type(TypeId::INTEGER)};
    result->names_ = {"column1"};

    return result;
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

// // DEBUG VERSION - NO
// unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
//   try {
//     LOG_INFO("preparing statement: %s", statement->ToString().c_str());
//     static int prepare_count = 0;
//     string prepare_name = "____bustub_internal_prepare_" + std::to_string(prepare_count);
//     prepare_count_++;
//     auto statement_query = statement->query_;
//     auto unbound_statement = statement->Copy();
//     unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(),
//     statement->query_); result->statement_type_ = StatementType::SELECT_STATEMENT; result->types_ =
//     {Type(TypeId::INTEGER)}; result->names_ = {"column1"}; return result;
//   } catch (Exception &ex) {
//     LOG_DEBUG("PREPARE FAILED");
//     return std::make_unique<PreparedStatement>(ex.what());
//   }
// }

}  // namespace bustub
