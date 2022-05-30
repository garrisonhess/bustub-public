#include "parser/expression/positional_reference_expression.h"
#include "planner/binder.h"
#include "planner/expression_binder.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(PositionalReferenceExpression &ref, uint64_t depth) {
  if (depth != 0) {
    return BindResult("Positional reference expression could not be bound");
  }
  return binder.bind_context.BindColumn(ref, depth);
}

}  // namespace bustub
