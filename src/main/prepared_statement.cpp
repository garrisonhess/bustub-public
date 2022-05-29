#include "main/prepared_statement.h"

#include "common/exception.h"
#include "common/macros.h"
#include "main/client_context.h"

namespace bustub {

PreparedStatement::PreparedStatement(shared_ptr<ClientContext> context, string query)
    : context_(move(context)), query_(move(query)), success_(true) {}

PreparedStatement::PreparedStatement(string error) : context_(nullptr), success_(false), error_(move(error)) {}

PreparedStatement::~PreparedStatement() = default;

int64_t PreparedStatement::ColumnCount() { return types_.size(); }

StatementType PreparedStatement::GetStatementType() { return statement_type_; }

const vector<Type> &PreparedStatement::GetTypes() { return types_; }

const vector<string> &PreparedStatement::GetNames() { return names_; }

unique_ptr<QueryResult> PreparedStatement::Execute() {
  auto stmt_type = StatementType::SELECT_STATEMENT;
  vector<Type> types = {Type(TypeId::INTEGER)};
  vector<string> names = {"column1"};
  unique_ptr<QueryResult> result = std::make_unique<QueryResult>(stmt_type, types, names);
  std::vector<Tuple> data = {};
  // context_->executor_->Execute(const AbstractPlanNode *plan, std::vector<Tuple> *result_set, Transaction *txn,
  // ExecutorContext *exec_ctx)

  context_->executor_->Execute(nullptr, &result->data_, nullptr, nullptr);
  result->success_ = true;

  vector<Value> temp_values = {Value(TypeId::INTEGER, 42069)};
  Column col = Column("column1", TypeId::INTEGER);
  const std::vector<Column> &columns = {col};
  Schema schema = Schema(columns);
  Tuple tup2 = Tuple(temp_values, &schema);
  result->data_.push_back(tup2);

  LOG_INFO("PreparedStatement Executing");
  return result;
  // return pending->Execute();
}

}  // namespace bustub
