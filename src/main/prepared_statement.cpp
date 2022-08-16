#include "main/prepared_statement.h"

#include "common/exception.h"
#include "common/macros.h"
#include "execution/execution_engine.h"
#include "execution/executor_factory.h"
#include "main/client_context.h"
#include "planner/plans/seq_scan_plan.h"

namespace bustub {

PreparedStatement::PreparedStatement(shared_ptr<ClientContext> context, string query)
    : context_(move(context)), query_(move(query)), success_(true) {}

PreparedStatement::PreparedStatement(string error) : context_(nullptr), success_(false), error_(move(error)) {}

int64_t PreparedStatement::ColumnCount() { return types_.size(); }

StatementType PreparedStatement::GetStatementType() { return statement_type_; }

const vector<Type> &PreparedStatement::GetTypes() { return types_; }

const vector<string> &PreparedStatement::GetNames() { return names_; }

unique_ptr<QueryResult> PreparedStatement::Execute() {
  auto stmt_type = StatementType::SELECT_STATEMENT;
  unique_ptr<QueryResult> result = std::make_unique<QueryResult>(stmt_type, types_, names_);

  // BEGIN
  LOG_INFO("PreparedStatement BEGIN");
  auto txn = context_->db_->GetTransactionManager().Begin();

  // EXECUTE
  LOG_INFO("PreparedStatement EXECUTE");
  auto engine = ExecutionEngine(*context_);
  engine.Execute(plan_.release(), &result->data_, txn);

  // COMMIT
  LOG_INFO("PreparedStatement COMMIT");
  context_->db_->GetTransactionManager().Commit(txn);

  return result;
}

}  // namespace bustub
