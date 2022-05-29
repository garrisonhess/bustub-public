// #include "parser/statement/explain_statement.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<ExplainStatement> Transformer::TransformExplain(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGExplainStmt *>(node);
// 	D_ASSERT(stmt);
// 	auto explain_type = ExplainType::EXPLAIN_STANDARD;
// 	if (stmt->options) {
// 		for (auto n = stmt->options->head; n; n = n->next) {
// 			auto def_elem = ((bustub_libpgquery::PGDefElem *)n->data.ptr_value)->defname;
// 			string elem(def_elem);
// 			if (elem == "analyze") {
// 				explain_type = ExplainType::EXPLAIN_ANALYZE;
// 			} else {
// 				throw NotImplementedException("Unimplemented explain type: %s", elem);
// 			}
// 		}
// 	}
// 	return make_unique<ExplainStatement>(TransformStatement(stmt->query), explain_type);
// }

// } // namespace bustub
