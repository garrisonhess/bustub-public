#include "parser/parsed_expression_iterator.h"

#include "parser/expression/list.h"
#include "parser/query_node.h"
#include "parser/query_node/select_node.h"
#include "parser/tableref/list.h"

namespace bustub {

void ParsedExpressionIterator::EnumerateChildren(const ParsedExpression &expression,
                                                 const std::function<void(const ParsedExpression &child)> &callback) {
  EnumerateChildren(const_cast<ParsedExpression &>(expression), [&](unique_ptr<ParsedExpression> &child) {
    assert(child);
    callback(*child);
  });
}

void ParsedExpressionIterator::EnumerateChildren(ParsedExpression &expr,
                                                 const std::function<void(ParsedExpression &child)> &callback) {
  EnumerateChildren(expr, [&](unique_ptr<ParsedExpression> &child) {
    assert(child);
    callback(*child);
  });
}

void ParsedExpressionIterator::EnumerateChildren(
    ParsedExpression &expr, const std::function<void(unique_ptr<ParsedExpression> &child)> &callback) {
  switch (expr.expression_class_) {
    case ExpressionClass::BOUND_EXPRESSION:
    case ExpressionClass::COLUMN_REF:
    case ExpressionClass::CONSTANT:
    case ExpressionClass::DEFAULT:
    case ExpressionClass::STAR:
    case ExpressionClass::PARAMETER:
    case ExpressionClass::POSITIONAL_REFERENCE:
      // these node types have no children
      break;
    case ExpressionClass::BETWEEN:
    case ExpressionClass::CASE:
    case ExpressionClass::CAST:
    case ExpressionClass::COLLATE:
    case ExpressionClass::COMPARISON:
    case ExpressionClass::CONJUNCTION:
    case ExpressionClass::FUNCTION:
    case ExpressionClass::LAMBDA:
    case ExpressionClass::OPERATOR:
    case ExpressionClass::SUBQUERY:
    case ExpressionClass::WINDOW:
    default:
      // called on non ParsedExpression type!
      throw NotImplementedException("Unimplemented expression class");
  }
}

void ParsedExpressionIterator::EnumerateQueryNodeModifiers(
    QueryNode &node, const std::function<void(unique_ptr<ParsedExpression> &child)> &callback) {
  for (auto &modifier : node.modifiers_) {
    switch (modifier->type_) {
      case ResultModifierType::LIMIT_MODIFIER: {
        auto &limit_modifier = (LimitModifier &)*modifier;
        if (limit_modifier.limit_) {
          callback(limit_modifier.limit_);
        }
        if (limit_modifier.offset_) {
          callback(limit_modifier.offset_);
        }
      } break;
      case ResultModifierType::ORDER_MODIFIER: {
        auto &order_modifier = (OrderModifier &)*modifier;
        for (auto &order : order_modifier.orders_) {
          callback(order.expression_);
        }
      } break;

      case ResultModifierType::DISTINCT_MODIFIER: {
        auto &distinct_modifier = (DistinctModifier &)*modifier;
        for (auto &target : distinct_modifier.distinct_on_targets_) {
          callback(target);
        }
      } break;

      // do nothing
      default:
        break;
    }
  }
}

void ParsedExpressionIterator::EnumerateTableRefChildren(
    TableRef &ref, const std::function<void(unique_ptr<ParsedExpression> &child)> &callback) {
  switch (ref.type_) {
    case TableReferenceType::EXPRESSION_LIST: {
      auto &el_ref = (ExpressionListRef &)ref;
      for (auto &value : el_ref.values_) {
        for (auto &j : value) {
          callback(j);
        }
      }
      break;
    }
    case TableReferenceType::JOIN: {
      auto &j_ref = (JoinRef &)ref;
      EnumerateTableRefChildren(*j_ref.left_, callback);
      EnumerateTableRefChildren(*j_ref.right_, callback);
      callback(j_ref.condition_);
      break;
    }
    case TableReferenceType::BASE_TABLE:
    case TableReferenceType::EMPTY:
      // these TableRefs do not need to be unfolded
      break;
    case TableReferenceType::CROSS_PRODUCT:
    case TableReferenceType::SUBQUERY:
    case TableReferenceType::TABLE_FUNCTION:
      throw NotImplementedException("TableRef type not implemented for traversal");
    default:
      throw NotImplementedException("TableRef type not implemented for traversal");
  }
}

void ParsedExpressionIterator::EnumerateQueryNodeChildren(
    QueryNode &node, const std::function<void(unique_ptr<ParsedExpression> &child)> &callback) {
  switch (node.type_) {
    case QueryNodeType::SELECT_NODE: {
      auto &sel_node = (SelectNode &)node;
      for (auto &i : sel_node.select_list_) {
        callback(i);
      }
      for (auto &group_expression : sel_node.groups_.group_expressions_) {
        callback(group_expression);
      }
      if (sel_node.where_clause_) {
        callback(sel_node.where_clause_);
      }
      if (sel_node.having_) {
        callback(sel_node.having_);
      }
      if (sel_node.qualify_) {
        callback(sel_node.qualify_);
      }

      EnumerateTableRefChildren(*sel_node.from_table_, callback);
      break;
    }
    case QueryNodeType::RECURSIVE_CTE_NODE:
    case QueryNodeType::SET_OPERATION_NODE:
      throw NotImplementedException("QueryNode type not implemented for traversal");
    default:
      throw NotImplementedException("QueryNode type not implemented for traversal");
  }

  if (!node.modifiers_.empty()) {
    EnumerateQueryNodeModifiers(node, callback);
  }
}

}  // namespace bustub
