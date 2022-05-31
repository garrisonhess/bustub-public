#include "planner/binder.h"
#include "planner/tableref/bound_basetableref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundBaseTableRef &ref) { return move(ref.get_); }

}  // namespace bustub
