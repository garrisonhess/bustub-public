#include "parser/statement/select_statement.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"

namespace bustub {

BoundStatement Binder::Bind(SelectStatement &stmt) { return Bind(*stmt.node_); }

}  // namespace bustub
