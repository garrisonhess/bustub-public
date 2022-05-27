#include "bustub/common/exception.hpp"
#include "bustub/parser/expression/constant_expression.hpp"
#include "bustub/parser/expression/function_expression.hpp"
#include "bustub/parser/expression/operator_expression.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformArrayAccess(bustub_libpgquery::PGAIndirection *indirection_node) {
	// transform the source expression
	unique_ptr<ParsedExpression> result;
	result = TransformExpression(indirection_node->arg);

	// now go over the indices
	// note that a single indirection node can contain multiple indices
	// this happens for e.g. more complex accesses (e.g. (foo).field1[42])
	idx_t list_size = 0;
	for (auto node = indirection_node->indirection->head; node != nullptr; node = node->next) {
		auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
		D_ASSERT(target);

		switch (target->type) {
		case bustub_libpgquery::T_PGAIndices: {
			// index access (either slice or extract)
			auto index = (bustub_libpgquery::PGAIndices *)target;
			vector<unique_ptr<ParsedExpression>> children;
			children.push_back(move(result));
			if (index->is_slice) {
				// slice
				children.push_back(!index->lidx ? make_unique<ConstantExpression>(Value())
				                                : TransformExpression(index->lidx));
				children.push_back(!index->uidx ? make_unique<ConstantExpression>(Value())
				                                : TransformExpression(index->uidx));
				result = make_unique<OperatorExpression>(ExpressionType::ARRAY_SLICE, move(children));
			} else {
				// array access
				D_ASSERT(!index->lidx);
				D_ASSERT(index->uidx);
				children.push_back(TransformExpression(index->uidx));
				result = make_unique<OperatorExpression>(ExpressionType::ARRAY_EXTRACT, move(children));
			}
			break;
		}
		case bustub_libpgquery::T_PGString: {
			auto val = (bustub_libpgquery::PGValue *)target;
			vector<unique_ptr<ParsedExpression>> children;
			children.push_back(move(result));
			children.push_back(TransformValue(*val));
			result = make_unique<OperatorExpression>(ExpressionType::STRUCT_EXTRACT, move(children));
			break;
		}
		default:
			throw NotImplementedException("Unimplemented subscript type");
		}
		list_size++;
		StackCheck(list_size);
	}
	return result;
}

} // namespace bustub
