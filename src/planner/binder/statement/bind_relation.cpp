#include "catalog/catalog.h"
#include "parser/expression/constant_expression.h"
#include "parser/query_node/select_node.h"
#include "parser/statement/insert_statement.h"
#include "parser/statement/relation_statement.h"
#include "planner/binder.h"

namespace bustub {

BoundStatement Binder::Bind(RelationStatement &stmt) { return stmt.relation->Bind(*this); }

}  // namespace bustub
