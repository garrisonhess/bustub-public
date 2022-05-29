// #include "parser/expression/conjunction_expression.h"
// #include "parser/expression/operator_expression.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<ParsedExpression> Transformer::TransformBoolExpr(bustub_libpgquery::PGBoolExpr *root) {
// 	unique_ptr<ParsedExpression> result;
// 	for (auto node = root->args->head; node != nullptr; node = node->next) {
// 		auto next = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value));

// 		switch (root->boolop) {
// 		case bustub_libpgquery::PG_AND_EXPR: {
// 			if (!result) {
// 				result = move(next);
// 			} else {
// 				result = make_unique<ConjunctionExpression>(ExpressionType::CONJUNCTION_AND,
// move(result), move(next));
// 			}
// 			break;
// 		}
// 		case bustub_libpgquery::PG_OR_EXPR: {
// 			if (!result) {
// 				result = move(next);
// 			} else {
// 				result = make_unique<ConjunctionExpression>(ExpressionType::CONJUNCTION_OR,
// move(result), move(next));
// 			}
// 			break;
// 		}
// 		case bustub_libpgquery::PG_NOT_EXPR: {
// 			if (next->type == ExpressionType::COMPARE_IN) {
// 				// convert COMPARE_IN to COMPARE_NOT_IN
// 				next->type = ExpressionType::COMPARE_NOT_IN;
// 				result = move(next);
// 			} else if (next->type >= ExpressionType::COMPARE_EQUAL &&
// 			           next->type <= ExpressionType::COMPARE_GREATERTHANOREQUALTO) {
// 				// NOT on a comparison: we can negate the comparison
// 				// e.g. NOT(x > y) is equivalent to x <= y
// 				next->type = NegateComparisionExpression(next->type);
// 				result = move(next);
// 			} else {
// 				result = make_unique<OperatorExpression>(ExpressionType::OPERATOR_NOT, move(next));
// 			}
// 			break;
// 		}
// 		}
// 	}
// 	return result;
// }

// } // namespace bustub
