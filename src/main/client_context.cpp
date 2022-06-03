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
#include "parser/parser.h"

#include <mutex>
#include <utility>

namespace bustub {

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) { db_ = std::move(database); }

// static void ExecuteCreateTable(Catalog &catalog, CreateStatement &stmt) {
//   Transaction txn = Transaction(123002);
//   vector<Column> columns = {};
//   for (auto &col : stmt.columns_) {
//     auto column = Column(col.name_, col.type_.GetTypeId());
//     columns.emplace_back(column);
//   }
//   auto schema = make_unique<Schema>(columns);
//   catalog.CreateTable(&txn, stmt.table_, *schema);
// }

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    try {
      LOG_INFO("statement type: %d", static_cast<int>(statement->type_));
      LOG_INFO("statement query_: %s", statement->query_.c_str());
      // LOG_INFO("preparing statement: %s", statement->ToString().c_str());
    } catch (Exception &ex) {
      LOG_INFO("couldn't print");
    }

    // ValidateStatement(*statement);

    // if (statement->type_ == StatementType::CREATE_STATEMENT) {
    //   Catalog &catalog = db_->GetCatalog();
    //   ExecuteCreateTable(catalog, dynamic_cast<CreateStatement &>(*statement));
    //   unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(), "");
    //   result->completed_ = true;
    //   return result;
    // }

    if (statement->type_ == StatementType::INSERT_STATEMENT) {
      // Catalog &catalog = db_->GetCatalog();
      // TableInfo *table = catalog.GetTable(statement.table_);
    }

    // auto unbound_statement = statement->Copy();

    // // Then just run the executor factory.
    // unique_ptr<PreparedStatement> result =
    //     std::make_unique<PreparedStatement>(shared_from_this(), unbound_statement->query_);
    // result->statement_type_ = statement->type_;
    return nullptr;
    // return result;
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

}  // namespace bustub
