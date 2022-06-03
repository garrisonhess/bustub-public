#include "common/exception.h"
#include "common/helper.h"
#include "planner/binder.h"
#include "planner/operator/logical_dummy_scan.h"
#include "planner/operator/logical_expression_get.h"
#include "planner/tableref/bound_expressionlistref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundExpressionListRef &ref) {
  auto root = MakeUniqueBase<LogicalOperator, LogicalDummyScan>(0);
  // values list, first plan any subqueries in the list
  // for (auto &expr_list : ref.values_) {
  //   for (auto &expr : expr_list) {
  //     PlanSubqueries(&expr, &root);
  //   }
  // }
  // now create a LogicalExpressionGet from the set of expressions
  // fetch the types
  vector<Type> types;
  for (auto &expr : ref.values_[0]) {
    types.push_back(expr->return_type_);
  }

  throw NotImplementedException("create plan");
  auto expr_get = make_unique<LogicalExpressionGet>(ref.bind_index_, types, move(ref.values_));
  expr_get->AddChild(move(root));
  return move(expr_get);
}

}  // namespace bustub
