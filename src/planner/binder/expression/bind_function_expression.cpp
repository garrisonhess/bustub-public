#include "catalog/catalog.h"
#include "catalog/catalog_entry/scalar_function_catalog_entry.h"
#include "execution/expression_executor.h"
#include "parser/expression/function_expression.h"
#include "planner/binder.h"
#include "planner/expression/bound_cast_expression.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression/bound_function_expression.h"
#include "planner/expression_binder.h"

namespace bustub {

BindResult ExpressionBinder::BindExpression(FunctionExpression &function, uint64_t depth,
                                            unique_ptr<ParsedExpression> *expr_ptr) {
  // lookup the function in the catalog
  QueryErrorContext error_context(binder.root_statement, function.query_location);

  if (function.function_name == "unnest" || function.function_name == "unlist") {
    // special case, not in catalog
    // TODO make sure someone does not create such a function OR
    // have unnest live in catalog, too
    return BindUnnest(function, depth);
  }
  auto &catalog = Catalog::GetCatalog(context);
  auto func = catalog.GetEntry(context, CatalogType::SCALAR_FUNCTION_ENTRY, function.schema, function.function_name,
                               false, error_context);
  switch (func->type) {
    case CatalogType::SCALAR_FUNCTION_ENTRY:
      // scalar function
      return BindFunction(function, (ScalarFunctionCatalogEntry *)func, depth);
    case CatalogType::MACRO_ENTRY:
      // macro function
      return BindMacro(function, (ScalarMacroCatalogEntry *)func, depth, expr_ptr);
    default:
      // aggregate function
      return BindAggregate(function, (AggregateFunctionCatalogEntry *)func, depth);
  }
}

BindResult ExpressionBinder::BindFunction(FunctionExpression &function, ScalarFunctionCatalogEntry *func,
                                          uint64_t depth) {
  // bind the children of the function expression
  string error;
  for (uint64_t i = 0; i < function.children.size(); i++) {
    BindChild(function.children[i], depth, error);
  }
  if (!error.empty()) {
    return BindResult(error);
  }
  if (binder.GetBindingMode() == BindingMode::EXTRACT_NAMES) {
    return BindResult(make_unique<BoundConstantExpression>(Value(Type::SQLNULL)));
  }

  // all children bound successfully
  // extract the children and types
  vector<unique_ptr<Expression>> children;
  for (uint64_t i = 0; i < function.children.size(); i++) {
    auto &child = (BoundExpression &)*function.children[i];
    D_ASSERT(child.expr);
    children.push_back(move(child.expr));
  }
  unique_ptr<Expression> result =
      ScalarFunction::BindScalarFunction(context, *func, move(children), error, function.is_operator);
  if (!result) {
    throw BinderException(binder.FormatError(function, error));
  }
  return BindResult(move(result));
}

BindResult ExpressionBinder::BindAggregate(FunctionExpression &expr, AggregateFunctionCatalogEntry *function,
                                           uint64_t depth) {
  return BindResult(binder.FormatError(expr, UnsupportedAggregateMessage()));
}

BindResult ExpressionBinder::BindUnnest(FunctionExpression &expr, uint64_t depth) {
  return BindResult(binder.FormatError(expr, UnsupportedUnnestMessage()));
}

string ExpressionBinder::UnsupportedAggregateMessage() { return "Aggregate functions are not supported here"; }

string ExpressionBinder::UnsupportedUnnestMessage() { return "UNNEST not supported here"; }

}  // namespace bustub
