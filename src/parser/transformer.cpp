#include "parser/transformer.h"

#include "parser/expression/list.h"
#include "parser/statement/list.h"
#include "parser/tableref/emptytableref.h"

namespace bustub {

StackChecker::StackChecker(Transformer &transformer_p, uint64_t stack_usage_p)
    : transformer(transformer_p), stack_usage(stack_usage_p) {
	transformer.stack_depth += stack_usage;
}

StackChecker::~StackChecker() {
	transformer.stack_depth -= stack_usage;
}

StackChecker::StackChecker(StackChecker &&other) noexcept
    : transformer(other.transformer), stack_usage(other.stack_usage) {
	other.stack_usage = 0;
}

Transformer::Transformer(uint64_t max_expression_depth_p)
    : parent(nullptr), max_expression_depth(max_expression_depth_p), stack_depth(DConstants::INVALID_INDEX) {
}

Transformer::Transformer(Transformer *parent)
    : parent(parent), max_expression_depth(parent->max_expression_depth), stack_depth(DConstants::INVALID_INDEX) {
}

bool Transformer::TransformParseTree(bustub_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements) {
	InitializeStackCheck();
	for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
		SetParamCount(0);
		auto stmt = TransformStatement((bustub_libpgquery::PGNode *)entry->data.ptr_value);
		D_ASSERT(stmt);
		stmt->n_param = ParamCount();
		statements.push_back(move(stmt));
	}
	return true;
}

void Transformer::InitializeStackCheck() {
	stack_depth = 0;
}

StackChecker Transformer::StackCheck(uint64_t extra_stack) {
	auto node = this;
	while (node->parent) {
		node = node->parent;
	}
	D_ASSERT(node->stack_depth != DConstants::INVALID_INDEX);
	if (node->stack_depth + extra_stack >= max_expression_depth) {
		throw ParserException("Max expression depth limit of %lld exceeded. Use \"SET max_expression_depth TO x\" to "
		                      "increase the maximum expression depth.",
		                      max_expression_depth);
	}
	return StackChecker(*node, extra_stack);
}

unique_ptr<SQLStatement> Transformer::TransformStatement(bustub_libpgquery::PGNode *stmt) {
	auto result = TransformStatementInternal(stmt);
	result->n_param = ParamCount();
	return result;
}

unique_ptr<SQLStatement> Transformer::TransformStatementInternal(bustub_libpgquery::PGNode *stmt) {
	switch (stmt->type) {
	case bustub_libpgquery::T_PGRawStmt: {
		auto raw_stmt = (bustub_libpgquery::PGRawStmt *)stmt;
		auto result = TransformStatement(raw_stmt->stmt);
		if (result) {
			result->stmt_location = raw_stmt->stmt_location;
			result->stmt_length = raw_stmt->stmt_len;
		}
		return result;
	}
	case bustub_libpgquery::T_PGSelectStmt:
		return TransformSelect(stmt);
	case bustub_libpgquery::T_PGCreateStmt:
		return TransformCreateTable(stmt);
	case bustub_libpgquery::T_PGCreateSchemaStmt:
		return TransformCreateSchema(stmt);
	case bustub_libpgquery::T_PGViewStmt:
		return TransformCreateView(stmt);
	case bustub_libpgquery::T_PGCreateSeqStmt:
		return TransformCreateSequence(stmt);
	case bustub_libpgquery::T_PGCreateFunctionStmt:
		return TransformCreateFunction(stmt);
	case bustub_libpgquery::T_PGDropStmt:
		return TransformDrop(stmt);
	case bustub_libpgquery::T_PGInsertStmt:
		return TransformInsert(stmt);
	case bustub_libpgquery::T_PGCopyStmt:
		return TransformCopy(stmt);
	case bustub_libpgquery::T_PGTransactionStmt:
		return TransformTransaction(stmt);
	case bustub_libpgquery::T_PGDeleteStmt:
		return TransformDelete(stmt);
	case bustub_libpgquery::T_PGUpdateStmt:
		return TransformUpdate(stmt);
	case bustub_libpgquery::T_PGIndexStmt:
		return TransformCreateIndex(stmt);
	case bustub_libpgquery::T_PGAlterTableStmt:
		return TransformAlter(stmt);
	case bustub_libpgquery::T_PGRenameStmt:
		return TransformRename(stmt);
	case bustub_libpgquery::T_PGPrepareStmt:
		return TransformPrepare(stmt);
	case bustub_libpgquery::T_PGExecuteStmt:
		return TransformExecute(stmt);
	case bustub_libpgquery::T_PGDeallocateStmt:
		return TransformDeallocate(stmt);
	case bustub_libpgquery::T_PGCreateTableAsStmt:
		return TransformCreateTableAs(stmt);
	case bustub_libpgquery::T_PGPragmaStmt:
		return TransformPragma(stmt);
	case bustub_libpgquery::T_PGExportStmt:
		return TransformExport(stmt);
	case bustub_libpgquery::T_PGImportStmt:
		return TransformImport(stmt);
	case bustub_libpgquery::T_PGExplainStmt:
		return TransformExplain(stmt);
	case bustub_libpgquery::T_PGVacuumStmt:
		return TransformVacuum(stmt);
	case bustub_libpgquery::T_PGVariableShowStmt:
		return TransformShow(stmt);
	case bustub_libpgquery::T_PGVariableShowSelectStmt:
		return TransformShowSelect(stmt);
	case bustub_libpgquery::T_PGCallStmt:
		return TransformCall(stmt);
	case bustub_libpgquery::T_PGVariableSetStmt:
		return TransformSet(stmt);
	case bustub_libpgquery::T_PGCheckPointStmt:
		return TransformCheckpoint(stmt);
	case bustub_libpgquery::T_PGLoadStmt:
		return TransformLoad(stmt);
	case bustub_libpgquery::T_PGCreateEnumStmt:
		return TransformCreateEnum(stmt);
	case bustub_libpgquery::T_PGAlterSeqStmt:
		return TransformAlterSequence(stmt);
	default:
		throw NotImplementedException(NodetypeToString(stmt->type));
	}
	return nullptr;
}

} // namespace bustub
