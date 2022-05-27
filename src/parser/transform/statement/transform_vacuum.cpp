#include "bustub/parser/statement/vacuum_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<VacuumStatement> Transformer::TransformVacuum(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGVacuumStmt *>(node);
	D_ASSERT(stmt);
	(void)stmt;
	auto result = make_unique<VacuumStatement>();
	return result;
}

} // namespace bustub
