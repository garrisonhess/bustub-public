#include "parser/expression/constant_expression.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression_binder.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(ConstantExpression &expr, uint64_t depth) {
  return BindResult(make_unique<BoundConstantExpression>(expr.value));
}

}  // namespace bustub
