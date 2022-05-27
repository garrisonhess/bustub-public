// #include "parser/tableref/crossproductref.h"
#include "parser/tableref/emptytableref.h"
#include "parser/transformer.h"

#include <memory>
namespace bustub {

unique_ptr<TableRef> Transformer::TransformFrom(bustub_libpgquery::PGList *root) {
	if (root == nullptr) {
		return std::make_unique<EmptyTableRef>();
	}

	// if (root->length > 1) {
	// 	// Cross Product
	// 	auto result = std::make_unique<CrossProductRef>();
	// 	CrossProductRef *cur_root = result.get();
	// 	uint64_t list_size = 0;
	// 	for (auto node = root->head; node != nullptr; node = node->next) {
	// 		auto n = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
	// 		unique_ptr<TableRef> next = TransformTableRefNode(n);
	// 		if (!cur_root->left) {
	// 			cur_root->left = move(next);
	// 		} else if (!cur_root->right) {
	// 			cur_root->right = move(next);
	// 		} else {
	// 			auto old_res = move(result);
	// 			result = make_unique<CrossProductRef>();
	// 			result->left = move(old_res);
	// 			result->right = move(next);
	// 			cur_root = result.get();
	// 		}
	// 		list_size++;
	// 		StackCheck(list_size);
	// 	}
	// 	return move(result);
	// }

	auto n = reinterpret_cast<bustub_libpgquery::PGNode *>(root->head->data.ptr_value);
	return TransformTableRefNode(n);
}

} // namespace bustub