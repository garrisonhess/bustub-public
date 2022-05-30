#include "parser/statement/create_statement.h"

namespace bustub {

CreateStatement::CreateStatement() : SQLStatement(StatementType::CREATE_STATEMENT) {}

CreateStatement::CreateStatement(const CreateStatement &other) = default;

unique_ptr<SQLStatement> CreateStatement::Copy() const {
  return unique_ptr<CreateStatement>(new CreateStatement(*this));
}

}  // namespace bustub
