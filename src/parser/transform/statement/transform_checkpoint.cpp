// #include "parser/transformer.h"
// #include "parser/expression/function_expression.h"
// #include "parser/statement/call_statement.h"

// namespace bustub {

// unique_ptr<SQLStatement> Transformer::TransformCheckpoint(bustub_libpgquery::PGNode *node) {
// 	auto checkpoint = (bustub_libpgquery::PGCheckPointStmt *)node;

// 	vector<unique_ptr<ParsedExpression>> children;
// 	// transform into "CALL checkpoint()" or "CALL force_checkpoint()"
// 	auto result = make_unique<CallStatement>();
// 	result->function =
// 	    make_unique<FunctionExpression>(checkpoint->force ? "force_checkpoint" : "checkpoint", move(children));
// 	return move(result);
// }

// } // namespace bustub
