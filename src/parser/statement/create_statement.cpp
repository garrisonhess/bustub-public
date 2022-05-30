#include "parser/statement/create_statement.h"

namespace bustub {

CreateStatement::CreateStatement() : SQLStatement(StatementType::CREATE_STATEMENT) {}

CreateStatement::CreateStatement(const CreateStatement &other) {
  table_ = other.table_;
  query_ = other.query_->Copy();
  columns_ = other.columns_;
  schema_ = other.schema_;
  sql_ = other.sql_;
}

unique_ptr<SQLStatement> CreateStatement::Copy() const {
  return unique_ptr<CreateStatement>(new CreateStatement(*this));
}

}  // namespace bustub
