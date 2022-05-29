#include "main/prepared_statement.h"
#include "common/exception.h"
#include "main/client_context.h"
#include "main/prepared_statement_data.h"
#include "common/macros.h"


namespace bustub {

PreparedStatement::PreparedStatement(shared_ptr<ClientContext> context, shared_ptr<PreparedStatementData> data_p,
                                     string query, int64_t n_param)
    : context_(move(context)), data_(move(data_p)), query_(move(query)), success_(true), n_param_(n_param) {
  assert(data_ || !success_);
  LOG_INFO("hi from prepared statement constructor");
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

unique_ptr<QueryResult> PreparedStatement::Execute(vector<Value> &values, bool allow_stream_result) {
  // auto pending = PendingQuery(values, allow_stream_result);
  // if (!pending->success) {
  //   return make_unique<QueryResult>(pending->error);
  // }
  auto stmt_type = StatementType::SELECT_STATEMENT;
  vector<Type> types = {Type(TypeId::INTEGER)};
  vector<string> names = {"column1"};
  unique_ptr<QueryResult> result = std::make_unique<QueryResult>(stmt_type, types, names);
  result->success_ = true;

  vector<Value> temp_values = {Value(TypeId::INTEGER, 128)};
  Column col = Column("column1", TypeId::INTEGER);
  const std::vector<Column> &columns = {col};
  Schema schema = Schema(columns);
  unique_ptr<Tuple> tup2 = std::make_unique<Tuple>(temp_values, &schema);

  result->data_.push_back(std::move(tup2));

  LOG_INFO("PreparedStatement Executing");
  return result;
  // return pending->Execute();
}

// unique_ptr<QueryResult> PreparedStatement::PendingQuery(vector<Value> &values, bool allow_stream_result) {
//   if (!success_) {
//     throw Exception("Attempting to execute an unsuccessfully prepared statement!");
//   }
//   // D_ASSERT(data);
//   PendingQueryParameters parameters;
//   parameters.parameters = &values;
//   parameters.allow_stream_result = allow_stream_result && data_->properties.allow_stream_result;
//   auto result = context_->PendingQuery(query_, data_, parameters);
//   return result;
// }

}  // namespace bustub
