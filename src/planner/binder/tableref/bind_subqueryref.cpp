// #include "parser/tableref/subqueryref.h"
// #include "planner/binder.h"
// #include "planner/tableref/bound_subqueryref.h"

// namespace bustub {

// unique_ptr<BoundTableRef> Binder::Bind(SubqueryRef &ref) {
//   auto binder = Binder::CreateBinder(context_, this);
//   binder->can_contain_nulls_ = true;
//   binder->alias_ = ref.alias_.empty() ? "unnamed_subquery" : ref.alias_;
//   auto subquery = binder->BindNode(*ref.subquery_->node_);
//   uint64_t bind_index = subquery->GetRootIndex();
//   string alias;
//   if (ref.alias_.empty()) {
//     alias = "unnamed_subquery" + std::to_string(bind_index);
//   } else {
//     alias = ref.alias_;
//   }
//   auto result = make_unique<BoundSubqueryRef>(move(binder), move(subquery));
//   bind_context_.AddSubquery(bind_index, alias, ref, *result->subquery_);
//   MoveCorrelatedExpressions(*result->binder_);
//   return move(result);
// }

// }  // namespace bustub
