// #include "parser/statement/pragma_statement.h"

// namespace bustub {

// PragmaStatement::PragmaStatement() : SQLStatement(StatementType::PRAGMA_STATEMENT), info(make_unique<PragmaInfo>()) {
// }

// PragmaStatement::PragmaStatement(const PragmaStatement &other) : SQLStatement(other), info(other.info->Copy()) {
// }

// unique_ptr<SQLStatement> PragmaStatement::Copy() const {
// 	return unique_ptr<PragmaStatement>(new PragmaStatement(*this));
// }

// } // namespace bustub