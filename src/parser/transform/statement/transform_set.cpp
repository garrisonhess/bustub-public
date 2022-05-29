// #include "parser/statement/set_statement.h"

// #include "parser/transformer.h"
// #include "parser/expression/constant_expression.h"

// namespace bustub {

// namespace {

// SetScope ToSetScope(bustub_libpgquery::VariableSetScope pg_scope) {
// 	switch (pg_scope) {
// 	case bustub_libpgquery::VariableSetScope::VAR_SET_SCOPE_LOCAL:
// 		return SetScope::LOCAL;
// 	case bustub_libpgquery::VariableSetScope::VAR_SET_SCOPE_SESSION:
// 		return SetScope::SESSION;
// 	case bustub_libpgquery::VariableSetScope::VAR_SET_SCOPE_GLOBAL:
// 		return SetScope::GLOBAL;
// 	case bustub_libpgquery::VariableSetScope::VAR_SET_SCOPE_DEFAULT:
// 		return SetScope::AUTOMATIC;
// 	default:
// 		throw InternalException("Unexpected pg_scope: %d", pg_scope);
// 	}
// }

// } // namespace

// unique_ptr<SetStatement> Transformer::TransformSet(bustub_libpgquery::PGNode *node) {
// 	D_ASSERT(node->type == bustub_libpgquery::T_PGVariableSetStmt);
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGVariableSetStmt *>(node);

// 	if (stmt->kind != bustub_libpgquery::VariableSetKind::VAR_SET_VALUE) {
// 		throw ParserException("Can only SET a variable to a value");
// 	}

// 	if (stmt->scope == bustub_libpgquery::VariableSetScope::VAR_SET_SCOPE_LOCAL) {
// 		throw NotImplementedException("SET LOCAL is not implemented.");
// 	}

// 	auto name = std::string(stmt->name);
// 	D_ASSERT(!name.empty()); // parser protect us!
// 	if (stmt->args->length != 1) {
// 		throw ParserException("SET needs a single scalar value parameter");
// 	}
// 	D_ASSERT(stmt->args->head && stmt->args->head->data.ptr_value);
// 	D_ASSERT(((bustub_libpgquery::PGNode *)stmt->args->head->data.ptr_value)->type ==
// bustub_libpgquery::T_PGAConst);

// 	auto value = TransformValue(((bustub_libpgquery::PGAConst *)stmt->args->head->data.ptr_value)->val)->value;

// 	return make_unique<SetStatement>(name, value, ToSetScope(stmt->scope));
// }

// } // namespace bustub
