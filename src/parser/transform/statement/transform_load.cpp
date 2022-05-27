#include "bustub/parser/statement/load_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<LoadStatement> Transformer::TransformLoad(bustub_libpgquery::PGNode *node) {
	D_ASSERT(node->type == bustub_libpgquery::T_PGLoadStmt);
	auto stmt = reinterpret_cast<bustub_libpgquery::PGLoadStmt *>(node);

	auto load_stmt = make_unique<LoadStatement>();
	auto load_info = make_unique<LoadInfo>();
	load_info->filename = std::string(stmt->filename);
	switch (stmt->load_type) {
	case bustub_libpgquery::PG_LOAD_TYPE_LOAD:
		load_info->load_type = LoadType::LOAD;
		break;
	case bustub_libpgquery::PG_LOAD_TYPE_INSTALL:
		load_info->load_type = LoadType::INSTALL;
		break;
	case bustub_libpgquery::PG_LOAD_TYPE_FORCE_INSTALL:
		load_info->load_type = LoadType::FORCE_INSTALL;
		break;
	}
	load_stmt->info = move(load_info);
	return load_stmt;
}

} // namespace bustub
