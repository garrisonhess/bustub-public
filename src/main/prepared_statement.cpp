#include "main/prepared_statement.h"
#include "common/exception.h"
#include "common/macros.h"
#include "main/client_context.h"
#include "main/prepared_statement_data.h"

namespace bustub {

PreparedStatement::PreparedStatement(shared_ptr<ClientContext> context, shared_ptr<PreparedStatementData> data_p,
                                     string query, int64_t n_param)
    : context_(move(context)), data_(move(data_p)), query_(move(query)), success_(true), n_param_(n_param) {
  assert(data_ || !success_);
}

PreparedStatement::PreparedStatement(string error) : context_(nullptr), success_(false), error_(move(error)) {}

PreparedStatement::~PreparedStatement() = default;

int64_t PreparedStatement::ColumnCount() {
  assert(data_);
  return data_->types_.size();
}

StatementType PreparedStatement::GetStatementType() {
  assert(data_);
  return data_->statement_type_;
}

StatementProperties PreparedStatement::GetStatementProperties() {
  assert(data_);
  return data_->properties_;
}

const vector<Type> &PreparedStatement::GetTypes() {
  assert(data_);
  return data_->types_;
}

const vector<string> &PreparedStatement::GetNames() {
  assert(data_);
  return data_->names_;
}

unique_ptr<QueryResult> PreparedStatement::Execute(vector<Value> &values) {
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
