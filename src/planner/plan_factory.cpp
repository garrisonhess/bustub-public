#include "planner/plan_factory.h"
#include "common/exception.h"
#include "common/logger.h"

namespace bustub {

std::unique_ptr<AbstractPlanNode> PlanFactory::CreatePlan(ClientContext &context, const SQLStatement &stmt) {
  LOG_INFO("create plan node w/ type: %d", static_cast<int>(stmt.type_));
  switch (stmt.type_) {
    case StatementType::SELECT_STATEMENT:
    case StatementType::INSERT_STATEMENT:
    case StatementType::UPDATE_STATEMENT:
    case StatementType::CREATE_STATEMENT:
    case StatementType::DELETE_STATEMENT:
      return nullptr;
    case StatementType::INVALID_STATEMENT:
    case StatementType::PREPARE_STATEMENT:
    case StatementType::EXECUTE_STATEMENT:
    case StatementType::ALTER_STATEMENT:
    case StatementType::TRANSACTION_STATEMENT:
    case StatementType::COPY_STATEMENT:
    case StatementType::ANALYZE_STATEMENT:
    case StatementType::VARIABLE_SET_STATEMENT:
    case StatementType::CREATE_FUNC_STATEMENT:
    case StatementType::EXPLAIN_STATEMENT:
    case StatementType::DROP_STATEMENT:
    case StatementType::EXPORT_STATEMENT:
    case StatementType::PRAGMA_STATEMENT:
    case StatementType::SHOW_STATEMENT:
    case StatementType::VACUUM_STATEMENT:
    case StatementType::CALL_STATEMENT:
    case StatementType::SET_STATEMENT:
    case StatementType::LOAD_STATEMENT:
    case StatementType::RELATION_STATEMENT:
      throw NotImplementedException("Plan node not implemented.");
  }

  return nullptr;
}

}  // namespace bustub
