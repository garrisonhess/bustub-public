#include "parser/statement/call_statement.h"
#include "parser/tableref/table_function_ref.h"
#include "planner/binder.h"
#include "planner/operator/logical_get.h"
#include "planner/tableref/bound_table_function.h"

namespace bustub {

BoundStatement Binder::Bind(CallStatement &stmt) {
  BoundStatement result;

  TableFunctionRef ref;
  ref.function = move(stmt.function);

  auto bound_func = Bind(ref);
  auto &bound_table_func = (BoundTableFunction &)*bound_func;
  auto &get = (LogicalGet &)*bound_table_func.get;
  D_ASSERT(get.returned_types.size() > 0);
  for (uint64_t i = 0; i < get.returned_types.size(); i++) {
    get.column_ids.push_back(i);
  }

  result.types = get.returned_types;
  result.names = get.names;
  result.plan = CreatePlan(*bound_func);
  properties.return_type = StatementReturnType::QUERY_RESULT;
  return result;
}

}  // namespace bustub
