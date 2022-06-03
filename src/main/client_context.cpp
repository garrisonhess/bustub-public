#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "execution/physical_plan_generator.h"
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

    auto unbound_statement = statement->Copy();

    // TODO(GH): figure out why it's select *
    LOG_INFO("About to call planner w/ query: %s", statement->query_.c_str());
    Planner planner = Planner(*this);
    planner.CreatePlan(move(statement));

    // PhysicalPlanGenerator gen = PhysicalPlanGenerator(*this);
    // auto physical_plan = gen.CreatePlan(move(planner.plan_));

    // LOG_INFO("Done w/ CreatePlan.");
    // // LOG_INFO("planner plan: \n%s", planner.plan_->ToString().c_str());
    // // LOG_INFO("planner binder_: \n%s", planner.binder_->ToString().c_str());
    // // LOG_INFO("planner context_: \n%s", planner.context_->ToString().c_str());
    // // LOG_INFO("planner names_: \n%s", planner.names_->ToString().c_str());
    // // LOG_INFO("planner types_: \n%s", planner.types_->ToString().c_str());

    unique_ptr<PreparedStatement> result =
        std::make_unique<PreparedStatement>(shared_from_this(), unbound_statement->query_);
    LOG_INFO("created Preparedstatement");
    result->statement_type_ = StatementType::SELECT_STATEMENT;
    result->types_ = {Type(TypeId::INTEGER)};
    result->names_ = {"column1"};
    // result->plan_ = physical_plan.release();

    // LOG_INFO("Returning result now!");
    return result;
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

}  // namespace bustub
