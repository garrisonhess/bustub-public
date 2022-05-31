#include "planner/logical_operator_visitor.h"

#include "common/exception.h"
#include "planner/expression/list.h"
#include "planner/expression_iterator.h"
#include "planner/operator/list.h"

namespace bustub {

void LogicalOperatorVisitor::VisitOperator(LogicalOperator &op) {
  VisitOperatorChildren(op);
  VisitOperatorExpressions(op);
}

void LogicalOperatorVisitor::VisitOperatorChildren(LogicalOperator &op) {
  for (auto &child : op.children_) {
    VisitOperator(*child);
  }
}

void LogicalOperatorVisitor::EnumerateExpressions(LogicalOperator &op,
                                                  const std::function<void(unique_ptr<Expression> *child)> &callback) {
  switch (op.type_) {
    case LogicalOperatorType::LOGICAL_EXPRESSION_GET:
    case LogicalOperatorType::LOGICAL_ORDER_BY:
    case LogicalOperatorType::LOGICAL_TOP_N:
    case LogicalOperatorType::LOGICAL_DISTINCT:
    case LogicalOperatorType::LOGICAL_DELIM_JOIN:
    case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
    case LogicalOperatorType::LOGICAL_ANY_JOIN:
    case LogicalOperatorType::LOGICAL_LIMIT:
    case LogicalOperatorType::LOGICAL_LIMIT_PERCENT:
    case LogicalOperatorType::LOGICAL_AGGREGATE_AND_GROUP_BY:
      throw NotImplementedException("operator type not implemented");
    default:
      break;
  }
  for (auto &expression : op.expressions_) {
    callback(&expression);
  }
}

void LogicalOperatorVisitor::VisitOperatorExpressions(LogicalOperator &op) {
  LogicalOperatorVisitor::EnumerateExpressions(op, [&](unique_ptr<Expression> *child) { VisitExpression(child); });
}

void LogicalOperatorVisitor::VisitExpression(unique_ptr<Expression> *expression) {
  auto &expr = **expression;
  unique_ptr<Expression> result;
  switch (expr.GetExpressionClass()) {
    case ExpressionClass::BOUND_COLUMN_REF:
      result = VisitReplace((BoundColumnRefExpression &)expr, expression);
      break;
    case ExpressionClass::BOUND_CONSTANT:
      result = VisitReplace((BoundConstantExpression &)expr, expression);
      break;
    case ExpressionClass::BOUND_REF:
      result = VisitReplace((BoundReferenceExpression &)expr, expression);
      break;
    case ExpressionClass::BOUND_DEFAULT:
      result = VisitReplace((BoundDefaultExpression &)expr, expression);
      break;
    case ExpressionClass::BOUND_COMPARISON:
    case ExpressionClass::BOUND_CONJUNCTION:
    case ExpressionClass::BOUND_WINDOW:
    case ExpressionClass::BOUND_UNNEST:
    case ExpressionClass::BOUND_AGGREGATE:
    case ExpressionClass::BOUND_BETWEEN:
    case ExpressionClass::BOUND_CASE:
    case ExpressionClass::BOUND_CAST:
      break;
    default:
      throw Exception("Unrecognized expression type in logical operator visitor");
  }
  if (result) {
    *expression = move(result);
  } else {
    // visit the children of this node
    VisitExpressionChildren(expr);
  }
}

void LogicalOperatorVisitor::VisitExpressionChildren(Expression &expr) {
  ExpressionIterator::EnumerateChildren(expr, [&](unique_ptr<Expression> &expr) { VisitExpression(&expr); });
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundAggregateExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundBetweenExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundCaseExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundCastExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundColumnRefExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundComparisonExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundConjunctionExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundConstantExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundDefaultExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundFunctionExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundOperatorExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundParameterExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundReferenceExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundSubqueryExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundWindowExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

unique_ptr<Expression> LogicalOperatorVisitor::VisitReplace(BoundUnnestExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
  return nullptr;
}

}  // namespace bustub
