#include "planner/plan_factory.h"
#include "common/exception.h"
#include "common/logger.h"
#include "common/macros.h"
#include "concurrency/transaction.h"
#include "parser/statement/insert_statement.h"
#include "parser/statement/select_statement.h"
#include "planner/expressions/abstract_expression.h"
#include "planner/plans/insert_plan.h"
#include "planner/plans/seq_scan_plan.h"

namespace bustub {
using std::make_unique;

std::unique_ptr<AbstractPlanNode> PlanFactory::CreatePlan(ClientContext &context, SQLStatement &stmt) {
  LOG_INFO("create plan node w/ type: %d", static_cast<int>(stmt.type_));
  switch (stmt.type_) {
    case StatementType::SELECT_STATEMENT: {
      auto select = dynamic_cast<const SelectStatement &>(stmt);
      Schema schema = Schema(select.columns_);
      TableInfo *table_info = context.db_->GetCatalog().GetTable(select.table_);
      if (table_info == nullptr) {
        throw Exception("SELECT table doesn't exist!");
      }

      // TODO(GH) parse and resolve (for WHERE clause)
      AbstractExpression *predicate = nullptr;
      return make_unique<SeqScanPlanNode>(&schema, predicate, table_info->oid_);
    }
    case StatementType::INSERT_STATEMENT: {
      // Note: we currently only support inserting from VALUES lists (as opposed to supporting insertion via SELECT).
      auto insert = dynamic_cast<InsertStatement *>(&stmt);
      TableInfo *table_info = context.db_->GetCatalog().GetTable(insert->table_);
      if (table_info == nullptr) {
        throw Exception("INSERT table doesn't exist!");
      }

      return make_unique<InsertPlanNode>(insert->values_, table_info->oid_);
    }
    case StatementType::DELETE_STATEMENT:
    //   return DeletePlanNode(const AbstractPlanNode *child, table_oid_t table_oid);
    case StatementType::UPDATE_STATEMENT:
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
    case StatementType::CREATE_STATEMENT:
      UNREACHABLE("Create statements must be completed before the planning phase (and never reach here).");
  }

  return nullptr;
}

}  // namespace bustub
