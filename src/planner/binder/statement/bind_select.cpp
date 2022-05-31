#include "parser/statement/select_statement.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"

namespace bustub {

BoundStatement Binder::Bind(SelectStatement &stmt) {
  LOG_INFO("About to convert select statement into a boundstatement");
  LOG_INFO("Stmt node: %s", stmt.node_->ToString().c_str());
  return Bind(*stmt.node_);
}

}  // namespace bustub
