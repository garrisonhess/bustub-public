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
//   PlanSubqueries(&condition, &root);
//   auto filter = make_unique<LogicalFilter>(move(condition));
//   filter->AddChild(move(root));
//   return move(filter);
// }

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSelectNode &statement) {
  unique_ptr<LogicalOperator> root;
  assert(statement.from_table_);
  root = CreatePlan(*statement.from_table_);
  assert(root);

  // if (statement.where_clause_) {
  //   root = PlanFilter(move(statement.where_clause_), move(root));
  // }

  // if (!statement.aggregates.empty() || !statement.groups.group_expressions.empty()) {
  // 	if (!statement.groups.group_expressions.empty()) {
  // 		// visit the groups
  // 		for (auto &group : statement.groups.group_expressions) {
  // 			PlanSubqueries(&group, &root);
  // 		}
  // 	}
  // 	// now visit all aggregate expressions
  // 	for (auto &expr : statement.aggregates) {
  // 		PlanSubqueries(&expr, &root);
  // 	}
  // 	// finally create the aggregate node with the group_index and aggregate_index as obtained from the binder
  // 	auto aggregate =
  // 	    make_unique<LogicalAggregate>(statement.group_index, statement.aggregate_index, move(statement.aggregates));
  // 	aggregate->groups = move(statement.groups.group_expressions);
  // 	aggregate->groupings_index = statement.groupings_index;
  // 	aggregate->grouping_sets = move(statement.groups.grouping_sets);
  // 	aggregate->grouping_functions = move(statement.grouping_functions);

  // 	aggregate->AddChild(move(root));
  // 	root = move(aggregate);
  // } else if (!statement.groups.grouping_sets.empty()) {
  // 	// edge case: we have grouping sets but no groups or aggregates
  // 	// this can only happen if we have e.g. select 1 from tbl group by ();
  // 	// just output a dummy scan
  // 	root = make_unique_base<LogicalOperator, LogicalDummyScan>(statement.group_index);
  // }

  // if (statement.having_) {
  //   PlanSubqueries(&statement.having_, &root);
  //   auto having = make_unique<LogicalFilter>(move(statement.having_));

  //   having->AddChild(move(root));
  //   root = move(having);
  // }

  // if (!statement.windows_.empty()) {
  // 	auto win = make_unique<LogicalWindow>(statement.window_index_);
  // 	win->expressions = move(statement.windows_);
  // 	// visit the window expressions
  // 	for (auto &expr : win->expressions) {
  // 		PlanSubqueries(&expr, &root);
  // 	}
  // 	D_ASSERT(!win->expressions.empty());
  // 	win->AddChild(move(root));
  // 	root = move(win);
  // }

  // if (statement.qualify_) {
  //   PlanSubqueries(&statement.qualify_, &root);
  //   auto qualify = make_unique<LogicalFilter>(move(statement.qualify_));

  //   qualify->AddChild(move(root));
  //   root = move(qualify);
  // }

  // if (!statement.unnests_.empty()) {
  // 	auto unnest = make_unique<LogicalUnnest>(statement.unnest_index_);
  // 	unnest->expressions = move(statement.unnests_);
  // 	// visit the unnest expressions
  // 	for (auto &expr : unnest->expressions) {
  // 		PlanSubqueries(&expr, &root);
  // 	}
  // 	assert(!unnest->expressions.empty());
  // 	unnest->AddChild(move(root));
  // 	root = move(unnest);
  // }

  for (auto &expr : statement.select_list_) {
    LOG_INFO("refusing to plan subqueries for expr: %s!", expr->ToString().c_str());
    // PlanSubqueries(&expr, &root);
  }

  LOG_INFO("Projection index: %lu", statement.projection_index_);
  // create the projection
  auto proj = make_unique<LogicalProjection>(statement.projection_index_, move(statement.select_list_));
  auto &projection = *proj;
  proj->AddChild(move(root));
  root = move(proj);

  // finish the plan by handling the elements of the QueryNode
  root = VisitQueryNode(statement, move(root));

  // add a prune node if necessary
  if (statement.need_prune_) {
    assert(root);
    vector<unique_ptr<Expression>> prune_expressions;
    for (uint64_t i = 0; i < statement.column_count_; i++) {
      prune_expressions.push_back(make_unique<BoundColumnRefExpression>(projection.expressions_[i]->return_type_,
                                                                        ColumnBinding(statement.projection_index_, i)));
    }
    auto prune = make_unique<LogicalProjection>(statement.prune_index_, move(prune_expressions));
    prune->AddChild(move(root));
    root = move(prune);
  }
  return root;
}

}  // namespace bustub
