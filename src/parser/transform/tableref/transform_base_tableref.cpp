#include "bustub/parser/tableref/basetableref.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<TableRef> Transformer::TransformRangeVar(bustub_libpgquery::PGRangeVar *root) {
	auto result = make_unique<BaseTableRef>();

	result->alias = TransformAlias(root->alias, result->column_name_alias);
	if (root->relname) {
		result->table_name = root->relname;
	}
	if (root->schemaname) {
		result->schema_name = root->schemaname;
	}
	if (root->sample) {
		result->sample = TransformSampleOptions(root->sample);
	}
	result->query_location = root->location;
	return move(result);
}

QualifiedName Transformer::TransformQualifiedName(bustub_libpgquery::PGRangeVar *root) {
	QualifiedName qname;
	if (root->relname) {
		qname.name = root->relname;
	} else {
		qname.name = string();
	}
	if (root->schemaname) {
		qname.schema = root->schemaname;
	} else {
		qname.schema = INVALID_SCHEMA;
	}
	return qname;
}

} // namespace bustub
