// #include "parser/expression/constant_expression.h"
// #include "parser/statement/copy_statement.h"
// #include "parser/transformer.h"
// #include "common/string_util.h"
// #include "common/types/value.h"
// #include "parser/tableref/basetableref.h"

// #include <cstring>

// namespace bustub {

// void Transformer::TransformCopyOptions(CopyInfo &info, bustub_libpgquery::PGList *options) {
// 	if (!options) {
// 		return;
// 	}
// 	bustub_libpgquery::PGListCell *cell = nullptr;

// 	// iterate over each option
// 	for_each_cell(cell, options->head) {
// 		auto *def_elem = reinterpret_cast<bustub_libpgquery::PGDefElem *>(cell->data.ptr_value);
// 		if (StringUtil::Lower(def_elem->defname) == "format") {
// 			// format specifier: interpret this option
// 			auto *format_val = (bustub_libpgquery::PGValue *)(def_elem->arg);
// 			if (!format_val || format_val->type != bustub_libpgquery::T_PGString) {
// 				throw ParserException("Unsupported parameter type for FORMAT: expected e.g. FORMAT 'csv',
// 'parquet'");
// 			}
// 			info.format = StringUtil::Lower(format_val->val.str);
// 			continue;
// 		}
// 		// otherwise
// 		if (info.options.find(def_elem->defname) != info.options.end()) {
// 			throw ParserException("Unexpected duplicate option \"%s\"", def_elem->defname);
// 		}
// 		if (!def_elem->arg) {
// 			info.options[def_elem->defname] = vector<Value>();
// 			continue;
// 		}
// 		switch (def_elem->arg->type) {
// 		case bustub_libpgquery::T_PGList: {
// 			auto column_list = (bustub_libpgquery::PGList *)(def_elem->arg);
// 			for (auto c = column_list->head; c != nullptr; c = lnext(c)) {
// 				auto target = (bustub_libpgquery::PGResTarget *)(c->data.ptr_value);
// 				info.options[def_elem->defname].push_back(Value(target->name));
// 			}
// 			break;
// 		}
// 		case bustub_libpgquery::T_PGAStar:
// 			info.options[def_elem->defname].push_back(Value("*"));
// 			break;
// 		default:
// 			info.options[def_elem->defname].push_back(
// 			    TransformValue(*((bustub_libpgquery::PGValue *)def_elem->arg))->value);
// 			break;
// 		}
// 	}
// }

// unique_ptr<CopyStatement> Transformer::TransformCopy(bustub_libpgquery::PGNode *node) {
// 	auto stmt = reinterpret_cast<bustub_libpgquery::PGCopyStmt *>(node);
// 	D_ASSERT(stmt);
// 	auto result = make_unique<CopyStatement>();
// 	auto &info = *result->info;

// 	// get file_path and is_from
// 	info.is_from = stmt->is_from;
// 	if (!stmt->filename) {
// 		// stdin/stdout
// 		info.file_path = info.is_from ? "/dev/stdin" : "/dev/stdout";
// 	} else {
// 		// copy to a file
// 		info.file_path = stmt->filename;
// 	}
// 	if (StringUtil::EndsWith(info.file_path, ".parquet")) {
// 		info.format = "parquet";
// 	} else {
// 		info.format = "csv";
// 	}

// 	// get select_list
// 	if (stmt->attlist) {
// 		for (auto n = stmt->attlist->head; n != nullptr; n = n->next) {
// 			auto target = reinterpret_cast<bustub_libpgquery::PGResTarget *>(n->data.ptr_value);
// 			if (target->name) {
// 				info.select_list.emplace_back(target->name);
// 			}
// 		}
// 	}

// 	if (stmt->relation) {
// 		auto ref = TransformRangeVar(stmt->relation);
// 		auto &table = *reinterpret_cast<BaseTableRef *>(ref.get());
// 		info.table = table.table_name;
// 		info.schema = table.schema_name;
// 	} else {
// 		result->select_statement = TransformSelectNode((bustub_libpgquery::PGSelectStmt *)stmt->query);
// 	}

// 	// handle the different options of the COPY statement
// 	TransformCopyOptions(info, stmt->options);

// 	return result;
// }

// } // namespace bustub
