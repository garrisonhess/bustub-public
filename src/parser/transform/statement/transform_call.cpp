// #include "parser/statement/call_statement.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<CallStatement> Transformer::TransformCall(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGCallStmt *>(node);
// 	// D_ASSERT(stmt);

// 	auto result = std::make_unique<CallStatement>();
// 	result->function = TransformFuncCall((bustub_libpgquery::PGFuncCall *)stmt->func);
// 	return result;
// }

// } // namespace bustub
