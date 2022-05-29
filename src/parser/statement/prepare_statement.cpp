#include "parser/statement/prepare_statement.h"
#include "common/constants.h"

namespace bustub {

PrepareStatement::PrepareStatement() : SQLStatement(StatementType::PREPARE_STATEMENT), statement_(nullptr) {}

PrepareStatement::PrepareStatement(const PrepareStatement &other)
    : SQLStatement(other), statement_(other.statement_->Copy()), name_(other.name_) {}

unique_ptr<SQLStatement> PrepareStatement::Copy() const {
  return unique_ptr<PrepareStatement>(new PrepareStatement(*this));
}

}  // namespace bustub
