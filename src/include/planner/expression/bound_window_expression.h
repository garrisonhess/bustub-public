//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression/bound_window_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "function/function.h"
#include "parser/expression/window_expression.h"
#include "planner/bound_query_node.h"
#include "planner/expression.h"

namespace bustub {
class AggregateFunction;

class BoundWindowExpression : public Expression {
 public:
  BoundWindowExpression(ExpressionType type, Type return_type, unique_ptr<AggregateFunction> aggregate,
                        unique_ptr<FunctionData> bind_info);

  //! The bound aggregate function
  unique_ptr<AggregateFunction> aggregate;
  //! The bound function info
  unique_ptr<FunctionData> bind_info;
  //! The child expressions of the main window aggregate
  vector<unique_ptr<Expression>> children;
  //! The set of expressions to partition by
  vector<unique_ptr<Expression>> partitions;
  //! The set of ordering clauses
  vector<BoundOrderByNode> orders;
  //! True to ignore NULL values
  bool ignore_nulls;
  //! The window boundaries
  WindowBoundary start = WindowBoundary::INVALID;
  WindowBoundary end = WindowBoundary::INVALID;

  unique_ptr<Expression> start_expr;
  unique_ptr<Expression> end_expr;
  //! Offset and default expressions for WINDOW_LEAD and WINDOW_LAG functions
  unique_ptr<Expression> offset_expr;
  unique_ptr<Expression> default_expr;

 public:
  bool IsWindow() const override { return true; }
  bool IsFoldable() const override { return false; }

  string ToString() const override;

  bool KeysAreCompatible(const BoundWindowExpression *other) const;
  bool Equals(const BaseExpression *other) const override;

  unique_ptr<Expression> Copy() override;
};
}  // namespace bustub
