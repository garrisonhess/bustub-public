#include "bustub/parser/statement/show_statement.hpp"
#include "bustub/parser/sql_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<ShowStatement> Transformer::TransformShowSelect(bustub_libpgquery::PGNode *node) {
	// we capture the select statement of SHOW
	auto stmt = reinterpret_cast<bustub_libpgquery::PGVariableShowSelectStmt *>(node);
	auto select_stmt = reinterpret_cast<bustub_libpgquery::PGSelectStmt *>(stmt->stmt);

	auto result = make_unique<ShowStatement>();
	auto &info = *result->info;
	info.is_summary = stmt->is_summary;

	info.query = TransformSelectNode(select_stmt);

	return result;
}

} // namespace bustub
