#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "execution/expressions/constant_value_expression.h"
#include "execution/physical_plan_generator.h"
#include "execution/plans/seq_scan_plan.h"
#include "main/database.h"
#include "main/query_result.h"
#include "parser/expression/constant_expression.h"
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
    Planner planner = Planner(*this);
    planner.CreatePlan(move(statement));
    LOG_INFO("planner plan: \n%s", planner.plan_->ToString().c_str());

    // Convert the logical plan to a BusTub plan.
    // planner.plan_.

    // Then just run the executor factory.
    unique_ptr<PreparedStatement> result =
        std::make_unique<PreparedStatement>(shared_from_this(), unbound_statement->query_);
    // result->statement_type_ = StatementType::SELECT_STATEMENT;
    // result->types_ = planner.types_;
    // result->names_ = planner.names_;
    // result->logical_ = planner.plan_.release();
    // result->plan_ = physical_plan.release();

    // // Setup data
    // const Value val = Value(TypeId::INTEGER, 1);
    // auto val_expr = ConstantValueExpression(val);
    // auto table_heap = std::make_unique<TableHeap>(*db_, 12345);
    // Column col1 = Column("hi", TypeId::INTEGER);
    // Schema schema = Schema({col1});
    // table_oid_t oid = 101;
    // TableInfo table_info = TableInfo(schema, "table1", move(table_heap), oid);
    // ConstantExpression constant_predicate = ConstantExpression(val);
    // AbstractExpression *predicate = dynamic_cast<AbstractExpression *>(&constant_predicate);

    // // Make plan node
    // auto plan_node = SeqScanPlanNode(&schema, predicate, oid);

    // // Execute plan
    // vector<Tuple> result_set = {};
    // Transaction txn = Transaction(10101);
    // db_->GetExecutionEngine().Execute(&plan_node, &result_set, &txn);

    return result;
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

}  // namespace bustub
