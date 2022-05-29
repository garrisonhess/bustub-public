// #include "parser/column_definition.h"
// #include "parser/constraint.h"
// #include "parser/constraints/list.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<Constraint> Transformer::TransformConstraint(bustub_libpgquery::PGListCell *cell) {
// 	auto constraint = reinterpret_cast<bustub_libpgquery::PGConstraint *>(cell->data.ptr_value);
// 	switch (constraint->contype) {
// 	case bustub_libpgquery::PG_CONSTR_UNIQUE:
// 	case bustub_libpgquery::PG_CONSTR_PRIMARY: {
// 		bool is_primary_key = constraint->contype == bustub_libpgquery::PG_CONSTR_PRIMARY;
// 		vector<string> columns;
// 		for (auto kc = constraint->keys->head; kc; kc = kc->next) {
// 			columns.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue
// *>(kc->data.ptr_value)->val.str);
// 		}
// 		return make_unique<UniqueConstraint>(columns, is_primary_key);
// 	}
// 	case bustub_libpgquery::PG_CONSTR_CHECK: {
// 		auto expression = TransformExpression(constraint->raw_expr);
// 		if (expression->HasSubquery()) {
// 			throw ParserException("subqueries prohibited in CHECK constraints");
// 		}
// 		return make_unique<CheckConstraint>(TransformExpression(constraint->raw_expr));
// 	}
// 	case bustub_libpgquery::PG_CONSTR_FOREIGN: {
// 		ForeignKeyInfo fk_info;
// 		fk_info.type = ForeignKeyType::FK_TYPE_FOREIGN_KEY_TABLE;
// 		if (constraint->pktable->schemaname) {
// 			fk_info.schema = constraint->pktable->schemaname;
// 		} else {
// 			fk_info.schema = "";
// 		}
// 		fk_info.table = constraint->pktable->relname;
// 		vector<string> pk_columns, fk_columns;
// 		for (auto kc = constraint->fk_attrs->head; kc; kc = kc->next) {
// 			fk_columns.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue
// *>(kc->data.ptr_value)->val.str);
// 		}
// 		if (constraint->pk_attrs) {
// 			for (auto kc = constraint->pk_attrs->head; kc; kc = kc->next) {
// 				pk_columns.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue
// *>(kc->data.ptr_value)->val.str);
// 			}
// 		}
// 		if (!pk_columns.empty() && pk_columns.size() != fk_columns.size()) {
// 			throw ParserException("The number of referencing and referenced columns for foreign keys must be the
// same");
// 		}
// 		if (fk_columns.empty()) {
// 			throw ParserException("The set of referencing and referenced columns for foreign keys must be not
// empty");
// 		}
// 		return make_unique<ForeignKeyConstraint>(pk_columns, fk_columns, move(fk_info));
// 	}
// 	default:
// 		throw NotImplementedException("Constraint type not handled yet!");
// 	}
// }

// unique_ptr<Constraint> Transformer::TransformConstraint(bustub_libpgquery::PGListCell *cell, ColumnDefinition
// &column,
//                                                         uint64_t index) {
// 	auto constraint = reinterpret_cast<bustub_libpgquery::PGConstraint *>(cell->data.ptr_value);
// 	D_ASSERT(constraint);
// 	switch (constraint->contype) {
// 	case bustub_libpgquery::PG_CONSTR_NOTNULL:
// 		return make_unique<NotNullConstraint>(index);
// 	case bustub_libpgquery::PG_CONSTR_CHECK:
// 		return TransformConstraint(cell);
// 	case bustub_libpgquery::PG_CONSTR_PRIMARY:
// 		return make_unique<UniqueConstraint>(index, true);
// 	case bustub_libpgquery::PG_CONSTR_UNIQUE:
// 		return make_unique<UniqueConstraint>(index, false);
// 	case bustub_libpgquery::PG_CONSTR_NULL:
// 		return nullptr;
// 	case bustub_libpgquery::PG_CONSTR_DEFAULT:
// 		column.default_value = TransformExpression(constraint->raw_expr);
// 		return nullptr;
// 	case bustub_libpgquery::PG_CONSTR_COMPRESSION:
// 		column.compression_type = CompressionTypeFromString(constraint->compression_name);
// 		if (column.compression_type == CompressionType::COMPRESSION_AUTO) {
// 			throw ParserException("Unrecognized option for column compression, expected none, uncompressed, rle,
// " 			                      "dictionary, pfor, bitpacking or fsst");
// 		}
// 		return nullptr;
// 	case bustub_libpgquery::PG_CONSTR_FOREIGN:
// 	default:
// 		throw NotImplementedException("Constraint not implemented!");
// 	}
// }

// } // namespace bustub
