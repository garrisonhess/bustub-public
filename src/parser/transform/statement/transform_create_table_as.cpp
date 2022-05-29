#include "parser/parsed_data/create_table_info.h"
#include "parser/statement/create_statement.h"
#include "parser/transformer.h"

namespace bustub {

// unique_ptr<CreateStatement> Transformer::TransformCreateTableAs(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGCreateTableAsStmt *>(node);
// 	// D_ASSERT(stmt);
// 	if (stmt->relkind == bustub_libpgquery::PG_OBJECT_MATVIEW) {
// 		throw NotImplementedException("Materialized view not implemented");
// 	}
// 	if (stmt->is_select_into || stmt->into->colNames || stmt->into->options) {
// 		throw NotImplementedException("Unimplemented features for CREATE TABLE as");
// 	}
// 	auto qname = TransformQualifiedName(stmt->into->rel);
// 	if (stmt->query->type != bustub_libpgquery::T_PGSelectStmt) {
// 		throw Exception("CREATE TABLE AS requires a SELECT clause");
// 	}
// 	auto query = TransformSelect(stmt->query, false);

// 	auto result = make_unique<CreateStatement>();
// 	auto info = make_unique<CreateTableInfo>();
// 	info->schema_ = qname.schema;
// 	info->table_ = qname.name;
// 	info->on_conflict_ = TransformOnConflict(stmt->onconflict);
// 	info->temporary_ =
// 	    stmt->into->rel->relpersistence == bustub_libpgquery::PGPostgresRelPersistence::PG_RELPERSISTENCE_TEMP;
// 	info->query_ = move(query);
// 	result->info = move(info);
// 	return result;
// }

}  // namespace bustub
