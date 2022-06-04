#include "main/prepared_statement.h"

#include "common/exception.h"
#include "common/macros.h"
#include "execution/execution_engine.h"
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
  vector<Type> types = {Type(TypeId::INTEGER)};
  vector<string> names = {"column1"};
  unique_ptr<QueryResult> result = std::make_unique<QueryResult>(stmt_type, types, names);
  Transaction txn = Transaction(101);
  LOG_INFO("PreparedStatement Executing");
  // LOG_INFO("plan pointer: %p", (void *)plan_);

  // ExecutionEngine engine = ExecutionEngine(*context_);
  // engine.Execute(plan_, &result->data_, &txn);

  LOG_INFO("PreparedStatement Done");

  Column col = Column(names.at(0), types.at(0).GetTypeId());
  const vector<Column> &columns = {col};
  Schema schema = Schema(columns);

  LOG_INFO("result length: %zu", result->data_.size());
  result->success_ = true;

  LOG_INFO("result data: ");
  for (auto &tuple : result->data_) {
    LOG_INFO("tuple: %s", tuple.ToString(&schema).c_str());
  }

  // vector<Value> temp_values = {Value(TypeId::INTEGER, 42069)};
  // Column col = Column("column1", TypeId::INTEGER);
  // const std::vector<Column> &columns = {col};
  // Schema schema = Schema(columns);
  // Tuple tup2 = Tuple(temp_values, &schema);
  // result->data_.push_back(tup2);

  return result;
}

}  // namespace bustub
