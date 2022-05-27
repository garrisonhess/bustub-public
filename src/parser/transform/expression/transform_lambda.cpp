#include "common/exception.h"
#include "parser/expression/lambda_expression.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformLambda(bustub_libpgquery::PGLambdaFunction *node) {
	if (!node->lhs) {
		throw ParserException("Lambda function must have parameters");
	}
	auto lhs = TransformExpression(node->lhs);
	auto rhs = TransformExpression(node->rhs);
	return make_unique<LambdaExpression>(move(lhs), move(rhs));
}

} // namespace bustub
