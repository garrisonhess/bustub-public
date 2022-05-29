// #include "parser/parsed_data/create_type_info.h"
// #include "parser/statement/create_statement.h"
// #include "parser/transformer.h"
// #include "common/types.h"
// #include "common/types/vector.h"

// namespace bustub {

// vector<string> ReadPgListToString(bustub_libpgquery::PGList *column_list) {
// 	vector<string> result;
// 	if (!column_list) {
// 		return result;
// 	}
// 	for (auto c = column_list->head; c != nullptr; c = lnext(c)) {
// 		auto target = (bustub_libpgquery::PGResTarget *)(c->data.ptr_value);
// 		result.emplace_back(target->name);
// 	}
// 	return result;
// }

// Vector ReadPgListToVector(bustub_libpgquery::PGList *column_list, uint64_t &size) {
// 	if (!column_list) {
// 		Vector result(Type::VARCHAR);
// 		return result;
// 	}
// 	// First we discover the size of this list
// 	for (auto c = column_list->head; c != nullptr; c = lnext(c)) {
// 		size++;
// 	}

// 	Vector result(Type::VARCHAR, size);
// 	auto result_ptr = FlatVector::GetData<string_t>(result);

// 	size = 0;
// 	for (auto c = column_list->head; c != nullptr; c = lnext(c)) {
// 		auto target = (bustub_libpgquery::PGResTarget *)(c->data.ptr_value);
// 		result_ptr[size++] = StringVector::AddStringOrBlob(result, target->name);
// 	}
// 	return result;
// }

// unique_ptr<CreateStatement> Transformer::TransformCreateEnum(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGCreateEnumStmt *>(node);
// 	D_ASSERT(stmt);
// 	auto result = make_unique<CreateStatement>();
// 	auto info = make_unique<CreateTypeInfo>();
// 	info->internal = false;
// 	info->name = ReadPgListToString(stmt->typeName)[0];
// 	uint64_t size = 0;
// 	auto ordered_array = ReadPgListToVector(stmt->vals, size);
// 	info->type = Type::ENUM(info->name, ordered_array, size);
// 	result->info = move(info);
// 	return result;
// }
// } // namespace bustub