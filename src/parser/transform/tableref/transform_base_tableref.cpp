#include "parser/tableref/basetableref.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<TableRef> Transformer::TransformRangeVar(bustub_libpgquery::PGRangeVar *root) {
  auto result = std::make_unique<BaseTableRef>();

  result->alias_ = TransformAlias(root->alias, result->column_name_alias_);
  if (root->relname != nullptr) {
    result->table_name_ = root->relname;
  }
  if (root->schemaname != nullptr) {
    result->schema_name_ = root->schemaname;
  }
  result->query_location_ = root->location;
  return result;
}

// QualifiedName Transformer::TransformQualifiedName(bustub_libpgquery::PGRangeVar *root) {
// 	QualifiedName qname;
// 	if (root->relname) {
// 		qname.name = root->relname;
// 	} else {
// 		qname.name = string();
// 	}
// 	if (root->schemaname) {
// 		qname.schema = root->schemaname;
// 	} else {
// 		qname.schema = INVALID_SCHEMA;
// 	}
// 	return qname;
// }

}  // namespace bustub
