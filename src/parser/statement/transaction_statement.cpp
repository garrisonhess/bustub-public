// #include "parser/statement/transaction_statement.h"

// namespace bustub {

// TransactionStatement::TransactionStatement(TransactionType type)
//     : SQLStatement(StatementType::TRANSACTION_STATEMENT), info(make_unique<TransactionInfo>(type)) {
// }

// TransactionStatement::TransactionStatement(const TransactionStatement &other)
//     : SQLStatement(other), info(make_unique<TransactionInfo>(other.info->type)) {
// }

// unique_ptr<SQLStatement> TransactionStatement::Copy() const {
// 	return unique_ptr<TransactionStatement>(new TransactionStatement(*this));
// }

// } // namespace bustub
