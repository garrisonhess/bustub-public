#include <algorithm>
#include "parser/statement/load_statement.h"
#include "planner/binder.h"
#include "planner/operator/logical_simple.h"

namespace bustub {

BoundStatement Binder::Bind(LoadStatement &stmt) {
  BoundStatement result;
  result.types = {Type::BOOLEAN};
  result.names = {"Success"};

  result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_LOAD, move(stmt.info));
  properties.allow_stream_result = false;
  properties.return_type = StatementReturnType::NOTHING;
  return result;
}

}  // namespace bustub
