#include "parser/statement/vacuum_statement.h"
#include "planner/binder.h"
#include "planner/operator/logical_simple.h"

namespace bustub {

BoundStatement Binder::Bind(VacuumStatement &stmt) {
  BoundStatement result;
  result.names = {"Success"};
  result.types = {LogicalType::BOOLEAN};
  result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_VACUUM, move(stmt.info));
  properties.return_type = StatementReturnType::NOTHING;
  return result;
}

}  // namespace bustub
