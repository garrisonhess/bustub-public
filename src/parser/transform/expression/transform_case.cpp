// #include "parser/expression/case_expression.h"
// #include "parser/expression/comparison_expression.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<ParsedExpression> Transformer::TransformCase(bustub_libpgquery::PGCaseExpr *root) {
// 	D_ASSERT(root);

// 	auto case_node = make_unique<CaseExpression>();
// 	for (auto cell = root->args->head; cell != nullptr; cell = cell->next) {
// 		CaseCheck case_check;

// 		auto w = reinterpret_cast<bustub_libpgquery::PGCaseWhen *>(cell->data.ptr_value);
// 		auto test_raw = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(w->expr));
// 		unique_ptr<ParsedExpression> test;
// 		auto arg = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(root->arg));
// 		if (arg) {
// 			case_check.when_expr =
// 			    make_unique<ComparisonExpression>(ExpressionType::COMPARE_EQUAL, move(arg), move(test_raw));
// 		} else {
// 			case_check.when_expr = move(test_raw);
// 		}
// 		case_check.then_expr = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(w->result));
// 		case_node->case_checks.push_back(move(case_check));
// 	}

// 	if (root->defresult) {
// 		case_node->else_expr = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(root->defresult));
// 	} else {
// 		case_node->else_expr = make_unique<ConstantExpression>(Value(Type::SQLNULL));
// 	}
// 	return move(case_node);
// }

// } // namespace bustub
