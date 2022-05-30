#include "planner/binder.h"
#include "planner/operator/logical_cross_product.h"
#include "planner/tableref/bound_crossproductref.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundCrossProductRef &expr) {
  auto cross_product = make_unique<LogicalCrossProduct>();

  auto left = CreatePlan(*expr.left);
  auto right = CreatePlan(*expr.right);

  cross_product->AddChild(move(left));
  cross_product->AddChild(move(right));

  return move(cross_product);
}

}  // namespace bustub
