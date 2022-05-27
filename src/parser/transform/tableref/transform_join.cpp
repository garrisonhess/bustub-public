#include "bustub/common/exception.hpp"
#include "bustub/parser/tableref/basetableref.hpp"
#include "bustub/parser/tableref/crossproductref.hpp"
#include "bustub/parser/tableref/joinref.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<TableRef> Transformer::TransformJoin(bustub_libpgquery::PGJoinExpr *root) {
	auto result = make_unique<JoinRef>();
	switch (root->jointype) {
	case bustub_libpgquery::PG_JOIN_INNER: {
		result->type = JoinType::INNER;
		break;
	}
	case bustub_libpgquery::PG_JOIN_LEFT: {
		result->type = JoinType::LEFT;
		break;
	}
	case bustub_libpgquery::PG_JOIN_FULL: {
		result->type = JoinType::OUTER;
		break;
	}
	case bustub_libpgquery::PG_JOIN_RIGHT: {
		result->type = JoinType::RIGHT;
		break;
	}
	case bustub_libpgquery::PG_JOIN_SEMI: {
		result->type = JoinType::SEMI;
		break;
	}
	default: {
		throw NotImplementedException("Join type %d not supported\n", root->jointype);
	}
	}

	// Check the type of left arg and right arg before transform
	result->left = TransformTableRefNode(root->larg);
	result->right = TransformTableRefNode(root->rarg);
	result->is_natural = root->isNatural;
	result->query_location = root->location;

	if (root->usingClause && root->usingClause->length > 0) {
		// usingClause is a list of strings
		for (auto node = root->usingClause->head; node != nullptr; node = node->next) {
			auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
			D_ASSERT(target->type == bustub_libpgquery::T_PGString);
			auto column_name = string(reinterpret_cast<bustub_libpgquery::PGValue *>(target)->val.str);
			result->using_columns.push_back(column_name);
		}
		return move(result);
	}

	if (!root->quals && result->using_columns.empty() && !result->is_natural) { // CROSS PRODUCT
		auto cross = make_unique<CrossProductRef>();
		cross->left = move(result->left);
		cross->right = move(result->right);
		return move(cross);
	}
	result->condition = TransformExpression(root->quals);
	return move(result);
}

} // namespace bustub
