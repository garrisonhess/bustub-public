#include "bustub/parser/statement/update_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<UpdateStatement> Transformer::TransformUpdate(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGUpdateStmt *>(node);
	D_ASSERT(stmt);

	auto result = make_unique<UpdateStatement>();

	result->table = TransformRangeVar(stmt->relation);
	if (stmt->fromClause) {
		result->from_table = TransformFrom(stmt->fromClause);
	}

	auto root = stmt->targetList;
	for (auto cell = root->head; cell != nullptr; cell = cell->next) {
		auto target = (bustub_libpgquery::PGResTarget *)(cell->data.ptr_value);
		result->columns.emplace_back(target->name);
		result->expressions.push_back(TransformExpression(target->val));
	}

	// Grab and transform the returning columns from the parser.
	if (stmt->returningList) {
		Transformer::TransformExpressionList(*(stmt->returningList), result->returning_list);
	}

	result->condition = TransformExpression(stmt->whereClause);
	return result;
}

} // namespace bustub
