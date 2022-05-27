#include "common/exception.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/star_expression.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformStarExpression(bustub_libpgquery::PGNode *node) {
	auto star = reinterpret_cast<bustub_libpgquery::PGAStar *>(node);
	auto result = std::make_unique<StarExpression>(star->relation != nullptr ? star->relation : string());
	if (star->except_list != nullptr) {
		for (auto head = star->except_list->head; head != nullptr; head = head->next) {
			auto value = static_cast<bustub_libpgquery::PGValue *>(head->data.ptr_value);
			// D_ASSERT(value->type == bustub_libpgquery::T_PGString);
			string exclude_entry = value->val.str;
			if (result->exclude_list.find(exclude_entry) != result->exclude_list.end()) {
				throw Exception("Duplicate entry \"%s\" in EXCLUDE list", exclude_entry);
			}
			result->exclude_list.insert(move(exclude_entry));
		}
	}
	if (star->replace_list != nullptr) {
		for (auto head = star->replace_list->head; head != nullptr; head = head->next) {
			auto list = static_cast<bustub_libpgquery::PGList *>(head->data.ptr_value);
			// D_ASSERT(list->length == 2);
			auto replace_expression = TransformExpression((bustub_libpgquery::PGNode *)list->head->data.ptr_value);
			auto value = static_cast<bustub_libpgquery::PGValue *>(list->tail->data.ptr_value);
			// D_ASSERT(value->type == bustub_libpgquery::T_PGString);
			string exclude_entry = value->val.str;
			if (result->replace_list.find(exclude_entry) != result->replace_list.end()) {
				throw Exception("Duplicate entry \"%s\" in REPLACE list", exclude_entry);
			}
			if (result->exclude_list.find(exclude_entry) != result->exclude_list.end()) {
				throw Exception("Column \"%s\" cannot occur in both EXCEPT and REPLACE list", exclude_entry);
			}
			result->replace_list.insert(make_pair(move(exclude_entry), move(replace_expression)));
		}
	}
	return move(result);
}

unique_ptr<ParsedExpression> Transformer::TransformColumnRef(bustub_libpgquery::PGColumnRef *root) {
	auto fields = root->fields;
	auto head_node = static_cast<bustub_libpgquery::PGNode *>(fields->head->data.ptr_value);
	switch (head_node->type) {
	case bustub_libpgquery::T_PGString: {
		if (fields->length < 1) {
			throw Exception("Unexpected field length");
		}
		vector<string> column_names;
		for (auto node = fields->head; node != nullptr; node = node->next) {
			column_names.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue *>(node->data.ptr_value)->val.str);
		}
		auto colref = make_unique<ColumnRefExpression>(move(column_names));
		colref->query_location = root->location;
		return move(colref);
	}
	case bustub_libpgquery::T_PGAStar: {
		return TransformStarExpression(head_node);
	}
	default:
		throw NotImplementedException("ColumnRef not implemented!");
	}
}

} // namespace bustub
