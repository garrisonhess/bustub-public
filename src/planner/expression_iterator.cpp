#include "planner/expression_iterator.h"

#include "planner/bound_query_node.h"
#include "planner/expression/list.h"
#include "planner/query_node/bound_select_node.h"
#include "planner/tableref/list.h"

namespace bustub {

void ExpressionIterator::EnumerateChildren(const Expression &expr,
                                           const std::function<void(const Expression &child)> &callback) {
  EnumerateChildren(const_cast<Expression &>(expr), [&](unique_ptr<Expression> &child) { callback(*child); });
}

void ExpressionIterator::EnumerateChildren(Expression &expr, const std::function<void(Expression &child)> &callback) {
  EnumerateChildren(expr, [&](unique_ptr<Expression> &child) { callback(*child); });
}

void ExpressionIterator::EnumerateChildren(Expression &expr,
                                           const std::function<void(unique_ptr<Expression> &child)> &callback) {
  switch (expr.expression_class_) {
    case ExpressionClass::BOUND_COLUMN_REF:
    case ExpressionClass::BOUND_CONSTANT:
    case ExpressionClass::BOUND_DEFAULT:
    case ExpressionClass::BOUND_PARAMETER:
    case ExpressionClass::BOUND_REF:
      // these node types have no children
      break;
    case ExpressionClass::BOUND_UNNEST:
    case ExpressionClass::BOUND_AGGREGATE:
    case ExpressionClass::BOUND_BETWEEN:
    case ExpressionClass::BOUND_CASE:
    case ExpressionClass::BOUND_CAST:
    case ExpressionClass::BOUND_COMPARISON:
    case ExpressionClass::BOUND_CONJUNCTION:
    case ExpressionClass::BOUND_FUNCTION:
    case ExpressionClass::BOUND_OPERATOR:
    case ExpressionClass::BOUND_WINDOW:
    case ExpressionClass::BOUND_SUBQUERY:
      throw Exception("ExpressionIterator used on unbound expression");
    default:
      throw Exception("ExpressionIterator used on unbound expression");
  }
}

void ExpressionIterator::EnumerateExpression(unique_ptr<Expression> &expr,
                                             const std::function<void(Expression &child)> &callback) {
  if (!expr) {
    return;
  }
  callback(*expr);
  ExpressionIterator::EnumerateChildren(*expr,
                                        [&](unique_ptr<Expression> &child) { EnumerateExpression(child, callback); });
}

void ExpressionIterator::EnumerateTableRefChildren(BoundTableRef &ref,
                                                   const std::function<void(Expression &child)> &callback) {
  switch (ref.type_) {
    case TableReferenceType::EXPRESSION_LIST: {
      auto &bound_expr_list = (BoundExpressionListRef &)ref;
      for (auto &expr_list : bound_expr_list.values_) {
        for (auto &expr : expr_list) {
          EnumerateExpression(expr, callback);
        }
      }
      break;
    }

    case TableReferenceType::EMPTY:
    case TableReferenceType::BASE_TABLE:
      break;
      //       case TableReferenceType::JOIN: {
      //   auto &bound_join = (BoundJoinRef &)ref;
      //   EnumerateExpression(bound_join.condition, callback);
      //   EnumerateTableRefChildren(*bound_join.left, callback);
      //   EnumerateTableRefChildren(*bound_join.right, callback);
      //   break;
      // }
    case TableReferenceType::JOIN:
    case TableReferenceType::TABLE_FUNCTION:
    case TableReferenceType::CTE:
    case TableReferenceType::SUBQUERY:
    case TableReferenceType::CROSS_PRODUCT:
      throw NotImplementedException("Unimplemented table reference type in ExpressionIterator");

    default:
      throw NotImplementedException("Unimplemented table reference type in ExpressionIterator");
  }
}

void ExpressionIterator::EnumerateQueryNodeChildren(BoundQueryNode &node,
                                                    const std::function<void(Expression &child)> &callback) {
  switch (node.type_) {
    case QueryNodeType::SELECT_NODE: {
      auto &bound_select = (BoundSelectNode &)node;
      for (auto &i : bound_select.select_list_) {
        EnumerateExpression(i, callback);
      }
      if (bound_select.from_table_) {
        EnumerateTableRefChildren(*bound_select.from_table_, callback);
      }
      break;
    }
    case QueryNodeType::SET_OPERATION_NODE:
    case QueryNodeType::RECURSIVE_CTE_NODE:
      throw NotImplementedException("Unimplemented query node in ExpressionIterator");
    default:
      throw NotImplementedException("Unimplemented query node in ExpressionIterator");
  }
  for (auto &modifier : node.modifiers_) {
    switch (modifier->type_) {
      case ResultModifierType::DISTINCT_MODIFIER:
        for (auto &expr : ((BoundDistinctModifier &)*modifier).target_distincts_) {
          EnumerateExpression(expr, callback);
        }
        break;
      case ResultModifierType::ORDER_MODIFIER:
        for (auto &order : ((BoundOrderModifier &)*modifier).orders_) {
          EnumerateExpression(order.expression_, callback);
        }
        break;
      default:
        break;
    }
  }
}

}  // namespace bustub
