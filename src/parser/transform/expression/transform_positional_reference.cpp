#include "bustub/common/exception.hpp"
#include "bustub/parser/transformer.hpp"
#include "bustub/parser/expression/positional_reference_expression.hpp"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformPositionalReference(bustub_libpgquery::PGPositionalReference *node) {
	if (node->position <= 0) {
		throw ParserException("Positional reference node needs to be >= 1");
	}
	auto result = make_unique<PositionalReferenceExpression>(node->position);
	result->query_location = node->location;
	return move(result);
}

} // namespace bustub
