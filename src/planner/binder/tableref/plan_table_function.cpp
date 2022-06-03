#include "planner/binder.h"
#include "planner/tableref/bound_table_function.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundTableFunction &ref) { return move(ref.get_); }

}  // namespace bustub
