#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "catalog/catalog.h"
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
#include "parser/statement/create_statement.h"
#include "planner/planner.h"

#include <mutex>
#include <utility>

namespace bustub {

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) { db_ = std::move(database); }

void ValidateCreate(const CreateStatement &stmt) {
  switch (stmt.type_) {
    case CatalogType::TABLE_ENTRY:
      break;
    case CatalogType::INDEX_ENTRY:
      throw NotImplementedException("todo index impl");
    case CatalogType::INVALID:
    case CatalogType::SCHEMA_ENTRY:
    case CatalogType::VIEW_ENTRY:
    case CatalogType::PREPARED_STATEMENT:
    case CatalogType::SEQUENCE_ENTRY:
    case CatalogType::COLLATION_ENTRY:
    case CatalogType::TYPE_ENTRY:
    case CatalogType::TABLE_FUNCTION_ENTRY:
    case CatalogType::SCALAR_FUNCTION_ENTRY:
    case CatalogType::AGGREGATE_FUNCTION_ENTRY:
    case CatalogType::PRAGMA_FUNCTION_ENTRY:
    case CatalogType::COPY_FUNCTION_ENTRY:
    case CatalogType::MACRO_ENTRY:
    case CatalogType::TABLE_MACRO_ENTRY:
    case CatalogType::UPDATED_ENTRY:
    case CatalogType::DELETED_ENTRY:
      throw NotImplementedException("Create is not implemented for this CatalogType!");
  }
}

void ValidateStatement(const SQLStatement &stmt) {
  switch (stmt.type_) {
    case StatementType::SELECT_STATEMENT:
      break;
    case StatementType::CREATE_STATEMENT:
      ValidateCreate(dynamic_cast<const CreateStatement &>(stmt));
      break;
    case StatementType::UPDATE_STATEMENT:
    case StatementType::INSERT_STATEMENT:
    case StatementType::DELETE_STATEMENT:
      throw NotImplementedException("TODO implement this statement type.");
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
      throw NotImplementedException("Statment type not implemented.");
  }
}

#include "catalog/column.h"

static void ExecuteCreateTable(Catalog &catalog, CreateStatement &stmt) {
  Transaction txn = Transaction(123002);
  vector<Column> columns = {};
  for (auto &col : stmt.columns_) {
    auto column = Column(col.name_, col.type_.GetTypeId());
    columns.emplace_back(column);
  }
  auto schema = make_unique<Schema>(columns);
  catalog.CreateTable(&txn, stmt.table_, *schema);

  // TableInfo *table = catalog.GetTable(stmt.table_);
  // LOG_INFO("table: %s", stmt.table_.c_str());
  // LOG_INFO("schema name of entry: %s", stmt.schema_.c_str());
  // LOG_INFO("sql_: %s", stmt.sql_.c_str());
  // LOG_INFO("num column defs: %zu", stmt.columns_.size());

  // for (auto &col : stmt.columns_) {
  //   LOG_INFO("col defn | Name: %s | oid: %ld | type: %d", col.name_.c_str(), col.oid_, col.type_.GetTypeId());
  // }
}

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    try {
      LOG_INFO("statement type: %hhu", statement->type_);
      LOG_INFO("statement query_: %s", statement->query_.c_str());
      LOG_INFO("preparing statement: %s", statement->ToString().c_str());
    } catch (Exception &ex) {
      LOG_INFO("couldn't print");
    }

    ValidateStatement(*statement);

    if (statement->type_ == StatementType::CREATE_STATEMENT) {
      Catalog &catalog = db_->GetCatalog();
      ExecuteCreateTable(catalog, dynamic_cast<CreateStatement &>(*statement));
      unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(), "");
      result->completed_ = true;
      return result;
    }

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
