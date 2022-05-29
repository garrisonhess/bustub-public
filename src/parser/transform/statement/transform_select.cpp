#include "parser/statement/select_statement.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<SelectStatement> Transformer::TransformSelect(bustub_libpgquery::PGNode *node, bool is_select) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGSelectStmt *>(node);
	auto result = std::make_unique<SelectStatement>();

	// Both Insert/Create Table As uses this.
	if (is_select) {
		if (stmt->intoClause != nullptr) {
			throw Exception("SELECT INTO not supported!");
		}
		if (stmt->lockingClause != nullptr) {
			throw Exception("SELECT locking clause is not supported!");
		}
	}

	result->node = TransformSelectNode(stmt);
	return result;
}

} // namespace bustub
