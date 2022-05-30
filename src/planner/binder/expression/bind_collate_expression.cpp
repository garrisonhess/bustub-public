#include "parser/expression/collate_expression.h"
#include "planner/expression_binder.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(CollateExpression &expr, uint64_t depth) {
  // first try to bind the child of the cast expression
  string error = Bind(&expr.child, depth);
  if (!error.empty()) {
    return BindResult(error);
  }
  auto &child = (BoundExpression &)*expr.child;
  if (child.expr->return_type.id() != LogicalTypeId::VARCHAR) {
    throw BinderException("collations are only supported for type varchar");
  }
  child.expr->return_type = LogicalType::VARCHAR_COLLATION(expr.collation);
  return BindResult(move(child.expr));
}

}  // namespace bustub
