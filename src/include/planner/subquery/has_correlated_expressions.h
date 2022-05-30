//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/subquery/has_correlated_expressions.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/binder.h"
#include "planner/logical_operator.h"

namespace bustub {

//! Helper class to recursively detect correlated expressions inside a single LogicalOperator
class HasCorrelatedExpressions : public LogicalOperatorVisitor {
 public:
  explicit HasCorrelatedExpressions(const vector<CorrelatedColumnInfo> &correlated);

  void VisitOperator(LogicalOperator &op) override;

  bool has_correlated_expressions;

 protected:
  unique_ptr<Expression> VisitReplace(BoundColumnRefExpression &expr, unique_ptr<Expression> *expr_ptr) override;
  unique_ptr<Expression> VisitReplace(BoundSubqueryExpression &expr, unique_ptr<Expression> *expr_ptr) override;

  const vector<CorrelatedColumnInfo> &correlated_columns;
};

}  // namespace bustub
