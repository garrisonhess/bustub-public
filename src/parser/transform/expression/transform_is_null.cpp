// #include "common/exception.h"
// #include "parser/expression/operator_expression.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<ParsedExpression> Transformer::TransformNullTest(bustub_libpgquery::PGNullTest *root) {
// 	// assert(root);
// 	auto arg = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(root->arg));
// 	if (root->argisrow) {
// 		throw NotImplementedException("IS NULL argisrow");
// 	}
// 	ExpressionType expr_type = (root->nulltesttype == bustub_libpgquery::PG_IS_NULL)
// 	                               ? ExpressionType::OPERATOR_IS_NULL
// 	                               : ExpressionType::OPERATOR_IS_NOT_NULL;

// 	return unique_ptr<ParsedExpression>(new OperatorExpression(expr_type, move(arg)));
// }

// } // namespace bustub
