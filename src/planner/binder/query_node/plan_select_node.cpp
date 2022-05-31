#include "common/exception.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_reference_expression.h"
#include "planner/operator/list.h"
#include "planner/operator/logical_dummy_scan.h"
#include "planner/operator/logical_limit.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::PlanFilter(unique_ptr<Expression> condition, unique_ptr<LogicalOperator> root) {
  //   PlanSubqueries(&condition, &root);
  //   auto filter = make_unique<LogicalFilter>(move(condition));
  //   filter->AddChild(move(root));
  //   return move(filter);
  throw NotImplementedException("planfilter");
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSelectNode &statement) {
  throw NotImplementedException("");

  //   unique_ptr<LogicalOperator> root;
  //   assert(statement.from_table);
  //   root = CreatePlan(*statement.from_table);
  //   assert(root);

  //   if (statement.where_clause) {
  //     root = PlanFilter(move(statement.where_clause), move(root));
  //   }

  //   if (!statement.aggregates.empty() || !statement.groups.group_expressions.empty()) {
  //     if (!statement.groups.group_expressions.empty()) {
  //       // visit the groups
  //       for (auto &group : statement.groups.group_expressions) {
  //         PlanSubqueries(&group, &root);
  //       }
  //     }
  //     // now visit all aggregate expressions
  //     for (auto &expr : statement.aggregates) {
  //       PlanSubqueries(&expr, &root);
  //     }
  //     // finally create the aggregate node with the group_index and aggregate_index as obtained from the binder
  //     auto aggregate =
  //         make_unique<LogicalAggregate>(statement.group_index, statement.aggregate_index,
  //         move(statement.aggregates));
  //     aggregate->groups = move(statement.groups.group_expressions);
  //     aggregate->groupings_index = statement.groupings_index;
  //     aggregate->grouping_sets = move(statement.groups.grouping_sets);
  //     aggregate->grouping_functions = move(statement.grouping_functions);

  //     aggregate->AddChild(move(root));
  //     root = move(aggregate);
  //   } else if (!statement.groups.grouping_sets.empty()) {
  //     // edge case: we have grouping sets but no groups or aggregates
  //     // this can only happen if we have e.g. select 1 from tbl group by ();
  //     // just output a dummy scan
  //     root = make_unique_base<LogicalOperator, LogicalDummyScan>(statement.group_index);
  //   }

  //   if (statement.having) {
  //     PlanSubqueries(&statement.having, &root);
  //     auto having = make_unique<LogicalFilter>(move(statement.having));

  //     having->AddChild(move(root));
  //     root = move(having);
  //   }

  //   if (!statement.windows.empty()) {
  //     auto win = make_unique<LogicalWindow>(statement.window_index);
  //     win->expressions = move(statement.windows);
  //     // visit the window expressions
  //     for (auto &expr : win->expressions) {
  //       PlanSubqueries(&expr, &root);
  //     }
  //     assert(!win->expressions.empty());
  //     win->AddChild(move(root));
  //     root = move(win);
  //   }

  //   if (statement.qualify) {
  //     PlanSubqueries(&statement.qualify, &root);
  //     auto qualify = make_unique<LogicalFilter>(move(statement.qualify));

  //     qualify->AddChild(move(root));
  //     root = move(qualify);
  //   }

  //   if (!statement.unnests.empty()) {
  //     auto unnest = make_unique<LogicalUnnest>(statement.unnest_index);
  //     unnest->expressions = move(statement.unnests);
  //     // visit the unnest expressions
  //     for (auto &expr : unnest->expressions) {
  //       PlanSubqueries(&expr, &root);
  //     }
  //     assert(!unnest->expressions.empty());
  //     unnest->AddChild(move(root));
  //     root = move(unnest);
  //   }

  //   for (auto &expr : statement.select_list) {
  //     PlanSubqueries(&expr, &root);
  //   }

  //   // create the projection
  //   auto proj = make_unique<LogicalProjection>(statement.projection_index, move(statement.select_list));
  //   auto &projection = *proj;
  //   proj->AddChild(move(root));
  //   root = move(proj);

  //   // finish the plan by handling the elements of the QueryNode
  //   root = VisitQueryNode(statement, move(root));

  //   // add a prune node if necessary
  //   if (statement.need_prune) {
  //     assert(root);
  //     vector<unique_ptr<Expression>> prune_expressions;
  //     for (uint64_t i = 0; i < statement.column_count; i++) {
  //       prune_expressions.push_back(make_unique<BoundColumnRefExpression>(projection.expressions[i]->return_type,
  //                                                                         ColumnBinding(statement.projection_index,
  //                                                                         i)));
  //     }
  //     auto prune = make_unique<LogicalProjection>(statement.prune_index, move(prune_expressions));
  //     prune->AddChild(move(root));
  //     root = move(prune);
  //   }
  //   return root;
}

}  // namespace bustub
