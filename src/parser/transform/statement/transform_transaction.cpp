#include "bustub/parser/statement/transaction_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<TransactionStatement> Transformer::TransformTransaction(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGTransactionStmt *>(node);
	D_ASSERT(stmt);
	switch (stmt->kind) {
	case bustub_libpgquery::PG_TRANS_STMT_BEGIN:
	case bustub_libpgquery::PG_TRANS_STMT_START:
		return make_unique<TransactionStatement>(TransactionType::BEGIN_TRANSACTION);
	case bustub_libpgquery::PG_TRANS_STMT_COMMIT:
		return make_unique<TransactionStatement>(TransactionType::COMMIT);
	case bustub_libpgquery::PG_TRANS_STMT_ROLLBACK:
		return make_unique<TransactionStatement>(TransactionType::ROLLBACK);
	default:
		throw NotImplementedException("Transaction type %d not implemented yet", stmt->kind);
	}
}

} // namespace bustub
