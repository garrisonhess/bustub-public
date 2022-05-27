// #include "parser/statement/pragma_statement.h"
// #include "parser/transformer.h"
// #include "parser/statement/show_statement.h"
// #include "parser/query_node/select_node.h"
// #include "parser/expression/star_expression.h"
// #include "parser/tableref/basetableref.h"

// namespace bustub {

// unique_ptr<SQLStatement> Transformer::TransformShow(bustub_libpgquery::PGNode *node) {
// 	// we transform SHOW x into PRAGMA SHOW('x')

// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGVariableShowStmt *>(node);
// 	if (stmt->is_summary) {
// 		auto result = make_unique<ShowStatement>();
// 		auto &info = *result->info;
// 		info.is_summary = stmt->is_summary;

// 		auto select = make_unique<SelectNode>();
// 		select->select_list.push_back(make_unique<StarExpression>());
// 		auto basetable = make_unique<BaseTableRef>();
// 		basetable->table_name = stmt->name;
// 		select->from_table = move(basetable);

// 		info.query = move(select);
// 		return move(result);
// 	}

// 	auto result = make_unique<PragmaStatement>();
// 	auto &info = *result->info;

// 	string name = stmt->name;
// 	if (name == "tables") {
// 		// show all tables
// 		info.name = "show_tables";
// 	} else if (name == "__show_tables_expanded") {
// 		info.name = "show_tables_expanded";
// 	} else {
// 		// show one specific table
// 		info.name = "show";
// 		info.parameters.emplace_back(stmt->name);
// 	}

// 	return move(result);
// }

// } // namespace bustub
