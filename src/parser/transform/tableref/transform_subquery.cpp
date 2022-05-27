// #include "parser/tableref/subqueryref.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<TableRef> Transformer::TransformRangeSubselect(bustub_libpgquery::PGRangeSubselect *root) {
// 	Transformer subquery_transformer(this);
// 	auto subquery = subquery_transformer.TransformSelect(root->subquery);
// 	if (!subquery) {
// 		return nullptr;
// 	}
// 	auto result = make_unique<SubqueryRef>(move(subquery));
// 	result->alias = TransformAlias(root->alias, result->column_name_alias);
// 	if (root->sample) {
// 		result->sample = TransformSampleOptions(root->sample);
// 	}
// 	return move(result);
// }

// } // namespace bustub
