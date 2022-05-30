#include <algorithm>
#include "parser/statement/set_statement.h"
#include "planner/binder.h"
#include "planner/operator/logical_set.h"

namespace bustub {

BoundStatement Binder::Bind(SetStatement &stmt) {
  BoundStatement result;
  result.types = {Type::BOOLEAN};
  result.names = {"Success"};

  result.plan = make_unique<LogicalSet>(stmt.name, stmt.value, stmt.scope);
  properties.return_type = StatementReturnType::NOTHING;
  return result;
}

}  // namespace bustub
