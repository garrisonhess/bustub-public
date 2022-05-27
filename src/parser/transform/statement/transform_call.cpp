#include "bustub/parser/statement/call_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<CallStatement> Transformer::TransformCall(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGCallStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<CallStatement>();
	result->function = TransformFuncCall((bustub_libpgquery::PGFuncCall *)stmt->func);
	return result;
}

} // namespace bustub
