#include "planner/binder.h"
#include "planner/operator/logical_dummy_scan.h"
#include "planner/tableref/bound_dummytableref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundEmptyTableRef &ref) {
  return make_unique<LogicalDummyScan>(ref.bind_index);
}

}  // namespace bustub
