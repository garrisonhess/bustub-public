// #include "common/exception.h"
// #include "parser/tableref/table_function_ref.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<TableRef> Transformer::TransformRangeFunction(bustub_libpgquery::PGRangeFunction *root) {
// 	if (root->lateral) {
// 		throw NotImplementedException("LATERAL not implemented");
// 	}
// 	if (root->ordinality) {
// 		throw NotImplementedException("WITH ORDINALITY not implemented");
// 	}
// 	if (root->is_rowsfrom) {
// 		throw NotImplementedException("ROWS FROM() not implemented");
// 	}
// 	if (root->functions->length != 1) {
// 		throw NotImplementedException("Need exactly one function");
// 	}
// 	auto function_sublist = (bustub_libpgquery::PGList *)root->functions->head->data.ptr_value;
// 	// assert(function_sublist->length == 2);
// 	auto call_tree = (bustub_libpgquery::PGNode *)function_sublist->head->data.ptr_value;
// 	auto coldef = function_sublist->head->next->data.ptr_value;

// 	if (coldef) {
// 		throw NotImplementedException("Explicit column definition not supported yet");
// 	}
// 	// transform the function call
// 	auto result = make_unique<TableFunctionRef>();
// 	switch (call_tree->type) {
// 	case bustub_libpgquery::T_PGFuncCall: {
// 		auto func_call = (bustub_libpgquery::PGFuncCall *)call_tree;
// 		result->function = TransformFuncCall(func_call);
// 		result->query_location = func_call->location;
// 		break;
// 	}
// 	case bustub_libpgquery::T_PGSQLValueFunction:
// 		result->function = TransformSQLValueFunction((bustub_libpgquery::PGSQLValueFunction *)call_tree);
// 		break;
// 	default:
// 		throw ParserException("Not a function call or value function");
// 	}
// 	result->alias = TransformAlias(root->alias, result->column_name_alias);
// 	if (root->sample != nullptr) {
// 		result->sample = TransformSampleOptions(root->sample);
// 	}
// 	return std::move(result);
// }

// } // namespace bustub
