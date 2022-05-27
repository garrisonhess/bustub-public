#include "parser/statement/create_statement.h"
#include "parser/parsed_data/create_table_info.h"
#include "parser/transformer.h"
#include "parser/expression/collate_expression.h"

#include <memory>

namespace bustub {

string Transformer::TransformCollation(bustub_libpgquery::PGCollateClause *collate) {
	if (collate == nullptr) {
		return {};
	}
	string collation;
	for (auto c = collate->collname->head; c != nullptr; c = lnext(c)) {
		auto pgvalue = static_cast<bustub_libpgquery::PGValue *>(c->data.ptr_value);
		if (pgvalue->type != bustub_libpgquery::T_PGString) {
			throw Exception("Expected a string as collation type!");
		}
		auto collation_argument = string(pgvalue->val.str);
		if (collation.empty()) {
			collation = collation_argument;
		} else {
			collation += "." + collation_argument;
		}
	}
	return collation;
}

// OnCreateConflict Transformer::TransformOnConflict(bustub_libpgquery::PGOnCreateConflict conflict) {
// 	switch (conflict) {
// 	case bustub_libpgquery::PG_ERROR_ON_CONFLICT:
// 		return OnCreateConflict::ERROR_ON_CONFLICT;
// 	case bustub_libpgquery::PG_IGNORE_ON_CONFLICT:
// 		return OnCreateConflict::IGNORE_ON_CONFLICT;
// 	case bustub_libpgquery::PG_REPLACE_ON_CONFLICT:
// 		return OnCreateConflict::REPLACE_ON_CONFLICT;
// 	default:
// 		throw Exception("Unrecognized OnConflict type");
// 	}
// }

unique_ptr<ParsedExpression> Transformer::TransformCollateExpr(bustub_libpgquery::PGCollateClause *collate) {
	auto child = TransformExpression(collate->arg);
	auto collation = TransformCollation(collate);
	return make_unique<CollateExpression>(collation, move(child));
}

ColumnDefinition Transformer::TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef) {
	string colname;
	if (cdef->colname != nullptr) {
		colname = cdef->colname;
	}
	TypeId target_type = TransformTypeName(cdef->typeName);
	if (cdef->collClause != nullptr) {
		if (target_type.id() != LogicalTypeId::VARCHAR) {
			throw Exception("Only VARCHAR columns can have collations!");
		}
		target_type = LogicalType::VARCHAR_COLLATION(TransformCollation(cdef->collClause));
	}

	return ColumnDefinition(colname, target_type);
}

unique_ptr<CreateStatement> Transformer::TransformCreateTable(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGCreateStmt *>(node);
	// D_ASSERT(stmt);
	auto result = make_unique<CreateStatement>();
	auto info = make_unique<CreateTableInfo>();

	if (stmt->inhRelations != nullptr) {
		throw NotImplementedException("inherited relations not implemented");
	}
	// D_ASSERT(stmt->relation);

	info->schema_ = INVALID_SCHEMA;
	if (stmt->relation->schemaname != nullptr) {
		info->schema_ = stmt->relation->schemaname;
	}
	info->table_ = stmt->relation->relname;
	// info->on_conflict = TransformOnConflict(stmt->onconflict);
	info->temporary_ =
	    stmt->relation->relpersistence == bustub_libpgquery::PGPostgresRelPersistence::PG_RELPERSISTENCE_TEMP;

	if (info->temporary_ && stmt->oncommit != bustub_libpgquery::PGOnCommitAction::PG_ONCOMMIT_PRESERVE_ROWS &&
	    stmt->oncommit != bustub_libpgquery::PGOnCommitAction::PG_ONCOMMIT_NOOP) {
		throw NotImplementedException("Only ON COMMIT PRESERVE ROWS is supported");
	}
	if (stmt->tableElts == nullptr) {
		throw Exception("Table must have at least one column!");
	}

	for (auto c = stmt->tableElts->head; c != nullptr; c = lnext(c)) {
		auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(c->data.ptr_value);
		switch (node->type) {
		case bustub_libpgquery::T_PGColumnDef: {
			auto cdef = static_cast<bustub_libpgquery::PGColumnDef *>(c->data.ptr_value);
			auto centry = TransformColumnDefinition(cdef);
			// if (cdef->constraints != nullptr) {
			// 	for (auto constr = cdef->constraints->head; constr != nullptr; constr = constr->next) {
			// 		auto constraint = TransformConstraint(constr, centry, info->columns.size());
			// 		if (constraint) {
			// 			info->constraints.push_back(move(constraint));
			// 		}
			// 	}
			// }
			info->columns_.push_back(std::move(centry));
			break;
		}
		case bustub_libpgquery::T_PGConstraint: {
			// info->constraints.push_back(TransformConstraint(c));
			break;
		}
		default:
			throw NotImplementedException("ColumnDef type not handled yet");
		}
	}
	result->info = move(info);
	return result;
}

} // namespace bustub
