#include "planner/binder.h"
#include "planner/tableref/bound_subqueryref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSubqueryRef &ref) {
  // generate the logical plan for the subquery
  // this happens separately from the current LogicalPlan generation
  ref.binder_->plan_subquery_ = plan_subquery_;
  auto subquery = ref.binder_->CreatePlan(*ref.subquery_);
  if (ref.binder_->has_unplanned_subqueries_) {
    has_unplanned_subqueries_ = true;
  }
  return subquery;
}

}  // namespace bustub
