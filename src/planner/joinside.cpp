#include "planner/joinside.h"

#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_comparison_expression.h"
#include "planner/expression/bound_subquery_expression.h"
#include "planner/expression_iterator.h"

namespace bustub {

unique_ptr<Expression> JoinCondition::CreateExpression(JoinCondition cond) {
  auto bound_comparison = make_unique<BoundComparisonExpression>(cond.comparison, move(cond.left), move(cond.right));
  return move(bound_comparison);
}

JoinSide JoinSide::CombineJoinSide(JoinSide left, JoinSide right) {
  if (left == JoinSide::NONE) {
    return right;
  }
  if (right == JoinSide::NONE) {
    return left;
  }
  if (left != right) {
    return JoinSide::BOTH;
  }
  return left;
}

JoinSide JoinSide::GetJoinSide(uint64_t table_binding, unordered_set<uint64_t> &left_bindings,
                               unordered_set<uint64_t> &right_bindings) {
  if (left_bindings.find(table_binding) != left_bindings.end()) {
    // column references table on left side
    assert(right_bindings.find(table_binding) == right_bindings.end());
    return JoinSide::LEFT;
  } else {
    // column references table on right side
    assert(right_bindings.find(table_binding) != right_bindings.end());
    return JoinSide::RIGHT;
  }
}

JoinSide JoinSide::GetJoinSide(Expression &expression, unordered_set<uint64_t> &left_bindings,
                               unordered_set<uint64_t> &right_bindings) {
  if (expression.type == ExpressionType::BOUND_COLUMN_REF) {
    auto &colref = (BoundColumnRefExpression &)expression;
    if (colref.depth > 0) {
      throw Exception("Non-inner join on correlated columns not supported");
    }
    return GetJoinSide(colref.binding.table_index, left_bindings, right_bindings);
  }
  assert(expression.type != ExpressionType::BOUND_REF);
  if (expression.type == ExpressionType::SUBQUERY) {
    assert(expression.GetExpressionClass() == ExpressionClass::BOUND_SUBQUERY);
    auto &subquery = (BoundSubqueryExpression &)expression;
    JoinSide side = JoinSide::NONE;
    if (subquery.child) {
      side = GetJoinSide(*subquery.child, left_bindings, right_bindings);
    }
    // correlated subquery, check the side of each of correlated columns in the subquery
    for (auto &corr : subquery.binder->correlated_columns) {
      if (corr.depth > 1) {
        // correlated column has depth > 1
        // it does not refer to any table in the current set of bindings
        return JoinSide::BOTH;
      }
      auto correlated_side = GetJoinSide(corr.binding.table_index, left_bindings, right_bindings);
      side = CombineJoinSide(side, correlated_side);
    }
    return side;
  }
  JoinSide join_side = JoinSide::NONE;
  ExpressionIterator::EnumerateChildren(expression, [&](Expression &child) {
    auto child_side = GetJoinSide(child, left_bindings, right_bindings);
    join_side = CombineJoinSide(child_side, join_side);
  });
  return join_side;
}

JoinSide JoinSide::GetJoinSide(const unordered_set<uint64_t> &bindings, unordered_set<uint64_t> &left_bindings,
                               unordered_set<uint64_t> &right_bindings) {
  JoinSide side = JoinSide::NONE;
  for (auto binding : bindings) {
    side = CombineJoinSide(side, GetJoinSide(binding, left_bindings, right_bindings));
  }
  return side;
}

}  // namespace bustub
