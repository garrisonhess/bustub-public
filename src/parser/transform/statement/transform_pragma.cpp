// #include "parser/statement/pragma_statement.h"
// #include "parser/transformer.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/expression/comparison_expression.h"
// #include "execution/expression_executor.h"
// #include "parser/statement/set_statement.h"
// #include "common/case_insensitive_map.h"
// #include "parser/expression/columnref_expression.h"

// namespace bustub {

// unique_ptr<SQLStatement> Transformer::TransformPragma(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGPragmaStmt *>(node);

// 	auto result = make_unique<PragmaStatement>();
// 	auto &info = *result->info;

// 	info.name = stmt->name;
// 	// parse the arguments, if any
// 	if (stmt->args) {
// 		for (auto cell = stmt->args->head; cell != nullptr; cell = cell->next) {
// 			auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(cell->data.ptr_value);
// 			auto expr = TransformExpression(node);

// 			if (expr->type == ExpressionType::COMPARE_EQUAL) {
// 				auto &comp = (ComparisonExpression &)*expr;
// 				if (comp.right->type != ExpressionType::VALUE_CONSTANT) {
// 					throw ParserException("Named parameter requires a constant on the RHS");
// 				}
// 				if (comp.left->type != ExpressionType::COLUMN_REF) {
// 					throw ParserException("Named parameter requires a column reference on the LHS");
// 				}
// 				auto &columnref = (ColumnRefExpression &)*comp.left;
// 				auto &constant = (ConstantExpression &)*comp.right;
// 				info.named_parameters[columnref.GetName()] = constant.value;
// 			} else if (node->type == bustub_libpgquery::T_PGAConst) {
// 				auto constant = TransformConstant((bustub_libpgquery::PGAConst *)node);
// 				info.parameters.push_back(((ConstantExpression &)*constant).value);
// 			} else {
// 				info.parameters.emplace_back(expr->ToString());
// 			}
// 		}
// 	}
// 	// now parse the pragma type
// 	switch (stmt->kind) {
// 	case bustub_libpgquery::PG_PRAGMA_TYPE_NOTHING: {
// 		if (!info.parameters.empty() || !info.named_parameters.empty()) {
// 			throw InternalException("PRAGMA statement that is not a call or assignment cannot contain parameters");
// 		}
// 		break;
// 	case bustub_libpgquery::PG_PRAGMA_TYPE_ASSIGNMENT:
// 		if (info.parameters.size() != 1) {
// 			throw InternalException("PRAGMA statement with assignment should contain exactly one parameter");
// 		}
// 		if (!info.named_parameters.empty()) {
// 			throw InternalException("PRAGMA statement with assignment cannot have named parameters");
// 		}
// 		// SQLite does not distinguish between:
// 		// "PRAGMA table_info='integers'"
// 		// "PRAGMA table_info('integers')"
// 		// for compatibility, any pragmas that match the SQLite ones are parsed as calls
// 		case_insensitive_set_t sqlite_compat_pragmas {"table_info"};
// 		if (sqlite_compat_pragmas.find(info.name) != sqlite_compat_pragmas.end()) {
// 			break;
// 		}
// 		auto set_statement = make_unique<SetStatement>(info.name, info.parameters[0], SetScope::AUTOMATIC);
// 		return move(set_statement);
// 	}
// 	case bustub_libpgquery::PG_PRAGMA_TYPE_CALL:
// 		break;
// 	default:
// 		throw InternalException("Unknown pragma type");
// 	}

// 	return move(result);
// }

// } // namespace bustub
