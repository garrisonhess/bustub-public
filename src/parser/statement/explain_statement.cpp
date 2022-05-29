#include "parser/statement/explain_statement.h"

namespace bustub {

ExplainStatement::ExplainStatement(unique_ptr<SQLStatement> stmt, ExplainType explain_type)
    : SQLStatement(StatementType::EXPLAIN_STATEMENT), stmt_(move(stmt)), explain_type_(explain_type) {}

ExplainStatement::ExplainStatement(const ExplainStatement &other)
    : SQLStatement(other), stmt_(other.stmt_->Copy()), explain_type_(other.explain_type_) {}

unique_ptr<SQLStatement> ExplainStatement::Copy() const {
  return unique_ptr<ExplainStatement>(new ExplainStatement(*this));
}

}  // namespace bustub
