#include "bustub/parser/statement/pragma_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<PragmaStatement> Transformer::TransformImport(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGImportStmt *>(node);
	auto result = make_unique<PragmaStatement>();
	result->info->name = "import_database";
	result->info->parameters.emplace_back(stmt->filename);
	return result;
}

} // namespace bustub
