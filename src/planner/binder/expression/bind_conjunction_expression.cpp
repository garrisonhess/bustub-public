#include "parser/expression/conjunction_expression.h"
#include "planner/expression/bound_cast_expression.h"
#include "planner/expression/bound_conjunction_expression.h"
#include "planner/expression_binder.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(ConjunctionExpression &expr, uint64_t depth) {
  // first try to bind the children of the case expression
  string error;
  for (uint64_t i = 0; i < expr.children.size(); i++) {
    BindChild(expr.children[i], depth, error);
  }
  if (!error.empty()) {
    return BindResult(error);
  }
  // the children have been successfully resolved
  // cast the input types to boolean (if necessary)
  // and construct the bound conjunction expression
  auto result = make_unique<BoundConjunctionExpression>(expr.type);
  for (auto &child_expr : expr.children) {
    auto &child = (BoundExpression &)*child_expr;
    result->children.push_back(BoundCastExpression::AddCastToType(move(child.expr), Type::BOOLEAN));
  }
  // now create the bound conjunction expression
  return BindResult(move(result));
}

}  // namespace bustub
