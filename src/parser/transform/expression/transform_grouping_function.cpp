#include "parser/transformer.h"
#include "parser/expression/operator_expression.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformGroupingFunction(bustub_libpgquery::PGGroupingFunc *n) {
	auto op = make_unique<OperatorExpression>(ExpressionType::GROUPING_FUNCTION);
	for (auto node = n->args->head; node != nullptr; node = node->next) {
		auto n = static_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
		op->children.push_back(TransformExpression(n));
	}
	op->query_location = n->location;
	return move(op);
}

} // namespace bustub
