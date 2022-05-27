// #include "parser/statement/select_statement.h"
// #include "parser/transformer.h"
// #include "common/string_util.h"

// namespace bustub {

// unique_ptr<SelectStatement> Transformer::TransformSelect(bustub_libpgquery::PGNode *node, bool is_select) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGSelectStmt *>(node);
// 	auto result = make_unique<SelectStatement>();

// 	// Both Insert/Create Table As uses this.
// 	if (is_select) {
// 		if (stmt->intoClause) {
// 			throw ParserException("SELECT INTO not supported!");
// 		}
// 		if (stmt->lockingClause) {
// 			throw ParserException("SELECT locking clause is not supported!");
// 		}
// 	}

// 	result->node = TransformSelectNode(stmt);
// 	return result;
// }

// } // namespace bustub