#include "common/exception.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_reference_expression.h"
#include "planner/operator/list.h"
#include "planner/operator/logical_dummy_scan.h"
#include "planner/operator/logical_limit.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

// unique_ptr<LogicalOperator> Binder::PlanFilter(unique_ptr<Expression> condition, unique_ptr<LogicalOperator> root) {
//   //   PlanSubqueries(&condition, &root);
//   //   auto filter = make_unique<LogicalFilter>(move(condition));
//   //   filter->AddChild(move(root));
//   //   return move(filter);
//   throw NotImplementedException("planfilter");
// }

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSelectNode &statement) {
  LOG_INFO("IN Binder::CreatePlan for BoundSelectNode.");

  unique_ptr<LogicalOperator> root;
  assert(statement.from_table_);
  root = CreatePlan(*statement.from_table_);
  assert(root);

  // finish the plan by handling the elements of the QueryNode
  root = VisitQueryNode(statement, move(root));
  return root;
}

}  // namespace bustub
