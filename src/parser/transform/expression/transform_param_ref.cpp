#include "parser/expression/parameter_expression.h"
#include "parser/transformer.h"
#include "common/algorithm.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformParamRef(bustub_libpgquery::PGParamRef *node) {
	D_ASSERT(node);
	auto expr = make_unique<ParameterExpression>();
	if (node->number == 0) {
		expr->parameter_nr = ParamCount() + 1;
	} else {
		expr->parameter_nr = node->number;
	}
	SetParamCount(MaxValue<idx_t>(ParamCount(), expr->parameter_nr));
	return move(expr);
}

} // namespace bustub
