// #include "parser/statement/create_statement.h"
// #include "parser/transformer.h"
// #include "parser/parsed_data/create_view_info.h"

// namespace bustub {

// unique_ptr<CreateStatement> Transformer::TransformCreateView(bustub_libpgquery::PGNode *node) {
// 	D_ASSERT(node);
// 	D_ASSERT(node->type == bustub_libpgquery::T_PGViewStmt);

// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGViewStmt *>(node);
// 	D_ASSERT(stmt);
// 	D_ASSERT(stmt->view);

// 	auto result = make_unique<CreateStatement>();
// 	auto info = make_unique<CreateViewInfo>();

// 	if (stmt->view->schemaname) {
// 		info->schema = stmt->view->schemaname;
// 	}
// 	info->view_name = stmt->view->relname;
// 	info->temporary = !stmt->view->relpersistence;
// 	if (info->temporary) {
// 		info->schema = TEMP_SCHEMA;
// 	}
// 	info->on_conflict = TransformOnConflict(stmt->onconflict);

// 	info->query = TransformSelect(stmt->query, false);

// 	if (stmt->aliases && stmt->aliases->length > 0) {
// 		for (auto c = stmt->aliases->head; c != nullptr; c = lnext(c)) {
// 			auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(c->data.ptr_value);
// 			switch (node->type) {
// 			case bustub_libpgquery::T_PGString: {
// 				auto val = (bustub_libpgquery::PGValue *)node;
// 				info->aliases.emplace_back(val->val.str);
// 				break;
// 			}
// 			default:
// 				throw NotImplementedException("View projection type");
// 			}
// 		}
// 		if (info->aliases.empty()) {
// 			throw ParserException("Need at least one column name in CREATE VIEW projection list");
// 		}
// 	}

// 	if (stmt->options && stmt->options->length > 0) {
// 		throw NotImplementedException("VIEW options");
// 	}

// 	if (stmt->withCheckOption != bustub_libpgquery::PGViewCheckOption::PG_NO_CHECK_OPTION) {
// 		throw NotImplementedException("VIEW CHECK options");
// 	}
// 	result->info = move(info);
// 	return result;
// }

// } // namespace bustub
