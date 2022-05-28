#include "main/prepared_statement.h"
#include "common/exception.h"
#include "main/client_context.h"
#include "main/prepared_statement_data.h"

namespace bustub {

PreparedStatement::PreparedStatement(shared_ptr<ClientContext> context, shared_ptr<PreparedStatementData> data_p,
                                     string query, int64_t n_param)
    : context_(move(context)), data_(move(data_p)), query_(move(query)), success_(true), n_param_(n_param) {
  // D_ASSERT(data || !success);
  LOG_INFO("hi from prepared statement constructor");
}

PreparedStatement::PreparedStatement(string error) : context_(nullptr), success_(false), error_(move(error)) {}

PreparedStatement::~PreparedStatement() = default;

int64_t PreparedStatement::ColumnCount() {
  // D_ASSERT(data);
  return data_->types_.size();
}

StatementType PreparedStatement::GetStatementType() {
  // D_ASSERT(data);
  return data_->statement_type_;
}

StatementProperties PreparedStatement::GetStatementProperties() {
  // D_ASSERT(data);
  return data_->properties_;
}

const vector<Type> &PreparedStatement::GetTypes() {
  // D_ASSERT(data);
  return data_->types_;
}

const vector<string> &PreparedStatement::GetNames() {
  // D_ASSERT(data);
  return data_->names_;
}

// unique_ptr<QueryResult> PreparedStatement::Execute(vector<Value> &values, bool allow_stream_result) {
//   auto pending = PendingQuery(values, allow_stream_result);
//   if (!pending->success) {
//     return make_unique<MaterializedQueryResult>(pending->error);
//   }
//   return pending->Execute();
// }

// unique_ptr<PendingQueryResult> PreparedStatement::PendingQuery(vector<Value> &values, bool allow_stream_result) {
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
