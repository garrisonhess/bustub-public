// #include "parser/tableref/crossproductref.h"
// #include "planner/binder.h"
// #include "planner/tableref/bound_crossproductref.h"

// namespace bustub {

// unique_ptr<BoundTableRef> Binder::Bind(CrossProductRef &ref) {
// 	auto result = make_unique<BoundCrossProductRef>();
// 	result->left_binder = Binder::CreateBinder(context_, this);
// 	result->right_binder = Binder::CreateBinder(context_, this);
// 	auto &left_binder = *result->left_binder;
// 	auto &right_binder = *result->right_binder;

// 	result->left = left_binder.Bind(*ref.left_);
// 	result->right = right_binder.Bind(*ref.right_);

// 	bind_context_.AddContext(move(left_binder.bind_context_));
// 	bind_context_.AddContext(move(right_binder.bind_context_));
// 	MoveCorrelatedExpressions(left_binder);
// 	MoveCorrelatedExpressions(right_binder);
// 	return move(result);
// }

// }  // namespace bustub
