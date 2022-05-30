#include "planner/binder.h"
#include "planner/operator/logical_cteref.h"
#include "planner/tableref/bound_cteref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundCTERef &ref) {
  auto index = ref.bind_index;

  vector<LogicalType> types;
  for (auto &type : ref.types) {
    types.push_back(type);
  }

  return make_unique<LogicalCTERef>(index, ref.cte_index, types, ref.bound_columns);
}

}  // namespace bustub
