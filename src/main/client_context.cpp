#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "catalog/catalog.h"
#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "execution/expressions/constant_value_expression.h"
#include "execution/plans/insert_plan.h"
#include "execution/plans/seq_scan_plan.h"
#include "main/database.h"
#include "main/query_result.h"
#include "parser/parser.h"
#include "parser/statement/create_statement.h"

#include <mutex>
#include <utility>

namespace bustub {

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) { db_ = std::move(database); }

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  try {
    LOG_INFO("statement type: %d", static_cast<int>(statement->type_));
    LOG_INFO("statement query_: %s", statement->query_.c_str());
    if (statement->type_ == StatementType::CREATE_STATEMENT) {
      Catalog &catalog = db_->GetCatalog();
      CreateStatement &create = dynamic_cast<CreateStatement &>(*statement);
      Transaction txn = Transaction(123002);
      auto schema = make_unique<Schema>(create.columns_);
      catalog.CreateTable(&txn, create.table_, *schema);
      unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(), "");
      result->completed_ = true;
      return result;
    }

    std::string unbound_query = statement->query_;

    unique_ptr<PreparedStatement> result = std::make_unique<PreparedStatement>(shared_from_this(), unbound_query);
    result->statement_type_ = statement->type_;

    //  Create AbstractPlanNode
    // result->plan_ = InsertPlanNode(statement->values_, 12345);

    return result;
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

}  // namespace bustub
