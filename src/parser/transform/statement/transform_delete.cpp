// #include "parser/statement/delete_statement.h"
// #include "parser/transformer.h"
// #include "parser/tableref/crossproductref.h"

// namespace bustub {

// unique_ptr<DeleteStatement> Transformer::TransformDelete(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGDeleteStmt *>(node);
// 	assert(stmt);
// 	auto result = make_unique<DeleteStatement>();

// 	result->condition = TransformExpression(stmt->whereClause);
// 	result->table = TransformRangeVar(stmt->relation);
// 	if (result->table->type != TableReferenceType::BASE_TABLE) {
// 		throw Exception("Can only delete from base tables!");
// 	}
// 	if (stmt->usingClause) {
// 		for (auto n = stmt->usingClause->head; n != nullptr; n = n->next) {
// 			auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(n->data.ptr_value);
// 			auto using_entry = TransformTableRefNode(target);
// 			result->using_clauses.push_back(move(using_entry));
// 		}
// 	}

// 	if (stmt->returningList) {
// 		Transformer::TransformExpressionList(*(stmt->returningList), result->returning_list);
// 	}
// 	return result;
// }

// } // namespace bustub
