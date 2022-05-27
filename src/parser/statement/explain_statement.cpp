#include "parser/statement/explain_statement.h"

namespace bustub {

ExplainStatement::ExplainStatement(unique_ptr<SQLStatement> stmt, ExplainType explain_type)
    : SQLStatement(StatementType::EXPLAIN_STATEMENT), stmt(move(stmt)), explain_type(explain_type) {
}

ExplainStatement::ExplainStatement(const ExplainStatement &other)
    : SQLStatement(other), stmt(other.stmt->Copy()), explain_type(other.explain_type) {
}

unique_ptr<SQLStatement> ExplainStatement::Copy() const {
	return unique_ptr<ExplainStatement>(new ExplainStatement(*this));
}

} // namespace bustub
