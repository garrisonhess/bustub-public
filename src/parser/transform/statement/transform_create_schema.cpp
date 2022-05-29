// #include "parser/statement/create_statement.h"
// #include "parser/parsed_data/create_schema_info.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<CreateStatement> Transformer::TransformCreateSchema(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGCreateSchemaStmt *>(node);
// 	D_ASSERT(stmt);
// 	auto result = make_unique<CreateStatement>();
// 	auto info = make_unique<CreateSchemaInfo>();

// 	D_ASSERT(stmt->schemaname);
// 	info->schema = stmt->schemaname;
// 	info->on_conflict = TransformOnConflict(stmt->onconflict);

// 	if (stmt->schemaElts) {
// 		// schema elements
// 		for (auto cell = stmt->schemaElts->head; cell != nullptr; cell = cell->next) {
// 			auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(cell->data.ptr_value);
// 			switch (node->type) {
// 			case bustub_libpgquery::T_PGCreateStmt:
// 			case bustub_libpgquery::T_PGViewStmt:
// 			default:
// 				throw NotImplementedException("Schema element not supported yet!");
// 			}
// 		}
// 	}
// 	result->info = move(info);
// 	return result;
// }

// } // namespace bustub
