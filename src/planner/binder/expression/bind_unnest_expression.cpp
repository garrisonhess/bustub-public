#include "catalog/catalog_entry/aggregate_function_catalog_entry.h"
#include "main/client_context.h"
#include "parser/expression/function_expression.h"
#include "planner/binder.h"
#include "planner/expression/bound_aggregate_expression.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_unnest_expression.h"
#include "planner/expression_binder/aggregate_binder.h"
#include "planner/expression_binder/select_binder.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

BindResult SelectBinder::BindUnnest(FunctionExpression &function, uint64_t depth) {
  // bind the children of the function expression
  string error;
  if (function.children.size() != 1) {
    return BindResult(binder.FormatError(function, "Unnest() needs exactly one child expressions"));
  }
  BindChild(function.children[0], depth, error);
  if (!error.empty()) {
    // failed to bind
    // try to bind correlated columns manually
    if (!BindCorrelatedColumns(function.children[0])) {
      return BindResult(error);
    }
    auto bound_expr = (BoundExpression *)function.children[0].get();
    ExtractCorrelatedExpressions(binder, *bound_expr->expr);
  }
  auto &child = (BoundExpression &)*function.children[0];
  auto &child_type = child.expr->return_type;

  if (child_type.id() != LogicalTypeId::LIST && child_type.id() != LogicalTypeId::SQLNULL) {
    return BindResult(binder.FormatError(function, "Unnest() can only be applied to lists and NULL"));
  }

  if (depth > 0) {
    throw BinderException(binder.FormatError(function, "Unnest() for correlated expressions is not supported yet"));
  }

  auto return_type = LogicalType(LogicalTypeId::SQLNULL);
  if (child_type.id() == LogicalTypeId::LIST) {
    return_type = ListType::GetChildType(child_type);
  }

  auto result = make_unique<BoundUnnestExpression>(return_type);
  result->child = move(child.expr);

  auto unnest_index = node.unnests.size();
  node.unnests.push_back(move(result));

  // TODO what if we have multiple unnests in the same projection list? ignore for now

  // now create a column reference referring to the unnest
  auto colref = make_unique<BoundColumnRefExpression>(
      function.alias.empty() ? node.unnests[unnest_index]->ToString() : function.alias, return_type,
      ColumnBinding(node.unnest_index, unnest_index), depth);

  return BindResult(move(colref));
}

}  // namespace bustub
