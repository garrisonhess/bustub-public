#include "planner/binder.h"
#include "planner/operator/logical_get.h"
#include "planner/tableref/bound_basetableref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundBaseTableRef &ref) { return move(ref.get); }

}  // namespace bustub
