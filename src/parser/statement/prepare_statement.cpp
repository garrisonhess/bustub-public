#include "parser/statement/prepare_statement.h"
#include "common/constants.h"

namespace bustub {

PrepareStatement::PrepareStatement() : SQLStatement(StatementType::PREPARE_STATEMENT), statement(nullptr), name(INVALID_SCHEMA) {
}

PrepareStatement::PrepareStatement(const PrepareStatement &other)
    : SQLStatement(other), statement(other.statement->Copy()), name(other.name) {
}

unique_ptr<SQLStatement> PrepareStatement::Copy() const {
	return unique_ptr<PrepareStatement>(new PrepareStatement(*this));
}

} // namespace bustub
