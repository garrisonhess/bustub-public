#include "parser/statement/select_statement.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"

namespace bustub {

BoundStatement Binder::Bind(SelectStatement &stmt) {
  properties.allow_stream_result = true;
  properties.return_type = StatementReturnType::QUERY_RESULT;
  return Bind(*stmt.node);
}

}  // namespace bustub
