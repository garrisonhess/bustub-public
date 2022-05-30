#include "parser/tableref/subqueryref.h"
#include "planner/binder.h"
#include "planner/tableref/bound_subqueryref.h"

namespace bustub {

unique_ptr<BoundTableRef> Binder::Bind(SubqueryRef &ref, CommonTableExpressionInfo *cte) {
  auto binder = Binder::CreateBinder(context, this);
  binder->can_contain_nulls = true;
  if (cte) {
    binder->bound_ctes.insert(cte);
  }
  binder->alias = ref.alias.empty() ? "unnamed_subquery" : ref.alias;
  auto subquery = binder->BindNode(*ref.subquery->node);
  uint64_t bind_index = subquery->GetRootIndex();
  string alias;
  if (ref.alias.empty()) {
    alias = "unnamed_subquery" + to_string(bind_index);
  } else {
    alias = ref.alias;
  }
  auto result = make_unique<BoundSubqueryRef>(move(binder), move(subquery));
  bind_context.AddSubquery(bind_index, alias, ref, *result->subquery);
  MoveCorrelatedExpressions(*result->binder);
  return move(result);
}

}  // namespace bustub
