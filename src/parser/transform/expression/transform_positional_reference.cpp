// #include "common/exception.h"
// #include "parser/transformer.h"
// #include "parser/expression/positional_reference_expression.h"

// namespace bustub {

// unique_ptr<ParsedExpression> Transformer::TransformPositionalReference(bustub_libpgquery::PGPositionalReference
// *node) { 	if (node->position <= 0) { 		throw ParserException("Positional reference node needs to be >= 1");
// 	}
// 	auto result = make_unique<PositionalReferenceExpression>(node->position);
// 	result->query_location = node->location;
// 	return move(result);
// }

// } // namespace bustub
