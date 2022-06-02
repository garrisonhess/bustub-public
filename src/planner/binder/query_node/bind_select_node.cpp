#include "common/exception.h"
#include "common/limits.h"
#include "common/string_util.h"
// #include "execution/expression_executor.h"
#include "common/enums/order_type.h"
#include "main/config.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/constant_expression.h"
#include "parser/query_node/select_node.h"
#include "parser/tableref/emptytableref.h"
#include "parser/tableref/joinref.h"
#include "planner/binder.h"
#include "planner/expression_binder/column_alias_binder.h"
#include "planner/expression_binder/constant_binder.h"
#include "planner/expression_binder/group_binder.h"
#include "planner/expression_binder/having_binder.h"
#include "planner/expression_binder/order_binder.h"
#include "planner/expression_binder/select_binder.h"
#include "planner/expression_binder/where_binder.h"
#include "planner/query_node/bound_select_node.h"
#include "type/value_factory.h"

namespace bustub {
using std::make_unique;

unique_ptr<Expression> Binder::BindOrderExpression(OrderBinder &order_binder, unique_ptr<ParsedExpression> expr) {
  // we treat the Distinct list as a order by
  auto bound_expr = order_binder.Bind(move(expr));
  if (!bound_expr) {
    // DISTINCT ON non-integer constant
    // remove the expression from the DISTINCT ON list
    return nullptr;
  }
  assert(bound_expr->type_ == ExpressionType::BOUND_COLUMN_REF);
  return bound_expr;
}

unique_ptr<Expression> Binder::BindDelimiter(ClientContext &context, OrderBinder &order_binder,
                                             unique_ptr<ParsedExpression> delimiter, const Type &type,
                                             Value &delimiter_value) {
  auto new_binder = Binder::CreateBinder(context, this, true);
  if (delimiter->HasSubquery()) {
    return order_binder.CreateExtraReference(move(delimiter));
  }
  ExpressionBinder expr_binder(*new_binder, context, false);
  expr_binder.target_type_ = type;
  auto expr = expr_binder.Bind(delimiter);
  // if (expr->IsFoldable()) {
  //   //! this is a constant
  //   delimiter_value = ExpressionExecutor::EvaluateScalar(*expr).CastAs(type);
  //   return nullptr;
  // }
  return expr;
}

unique_ptr<BoundResultModifier> Binder::BindLimit(OrderBinder &order_binder, LimitModifier &limit_mod) {
  auto result = make_unique<BoundLimitModifier>();
  if (limit_mod.limit_) {
    Value val;
    result->limit_ = BindDelimiter(context_, order_binder, move(limit_mod.limit_), Type(TypeId::BIGINT), val);
    // if (!result->limit_) {
    //   result->limit_val_ = val.GetValue<int64_t>();
    //   if (result->limit_val_ < 0) {
    //     throw Exception("LIMIT cannot be negative");
    //   }
    // }
  }
  if (limit_mod.offset_) {
    Value val;
    result->offset_ = BindDelimiter(context_, order_binder, move(limit_mod.offset_), Type(TypeId::BIGINT), val);
    // if (!result->offset_) {
    //   result->offset_val_ = val.GetValue<int64_t>();
    //   if (result->offset_val_ < 0) {
    //     throw Exception("OFFSET cannot be negative");
    //   }
    // }
  }
  return move(result);
}

void Binder::BindModifiers(OrderBinder &order_binder, QueryNode &statement, BoundQueryNode &result) {
  for (auto &mod : statement.modifiers_) {
    unique_ptr<BoundResultModifier> bound_modifier;
    switch (mod->type_) {
      case ResultModifierType::DISTINCT_MODIFIER: {
        auto &distinct = (DistinctModifier &)*mod;
        auto bound_distinct = make_unique<BoundDistinctModifier>();
        if (distinct.distinct_on_targets_.empty()) {
          for (uint64_t i = 0; i < result.names_.size(); i++) {
            distinct.distinct_on_targets_.push_back(
                make_unique<ConstantExpression>(ValueFactory::GetIntegerValue(1 + i)));
          }
        }
        for (auto &distinct_on_target : distinct.distinct_on_targets_) {
          auto expr = BindOrderExpression(order_binder, move(distinct_on_target));
          if (!expr) {
            continue;
          }
          bound_distinct->target_distincts_.push_back(move(expr));
        }
        bound_modifier = move(bound_distinct);
        break;
      }
      case ResultModifierType::ORDER_MODIFIER: {
        auto &order = (OrderModifier &)*mod;
        auto bound_order = make_unique<BoundOrderModifier>();
        // auto &config = DBConfig::GetConfig(context_);
        assert(!order.orders_.empty());
        if (order.orders_[0].expression_->type_ == ExpressionType::STAR) {
          // ORDER BY ALL
          // replace the order list with the maximum order by count
          assert(order.orders_.size() == 1);
          auto order_type = order.orders_[0].type_;
          auto null_order = order.orders_[0].null_order_;

          vector<OrderByNode> new_orders;
          for (uint64_t i = 0; i < order_binder.MaxCount(); i++) {
            new_orders.emplace_back(order_type, null_order,
                                    make_unique<ConstantExpression>(ValueFactory::GetIntegerValue(i + 1)));
          }
          order.orders_ = move(new_orders);
        }
        for (auto &order_node : order.orders_) {
          auto order_expression = BindOrderExpression(order_binder, move(order_node.expression_));
          if (!order_expression) {
            continue;
          }
          auto type = order_node.type_ == OrderType::ORDER_DEFAULT ? OrderType::ASCENDING : order_node.type_;
          auto null_order = order_node.null_order_ == OrderByNullType::ORDER_DEFAULT ? OrderByNullType::NULLS_FIRST
                                                                                     : order_node.null_order_;
          bound_order->orders_.emplace_back(type, null_order, move(order_expression));
        }
        if (!bound_order->orders_.empty()) {
          bound_modifier = move(bound_order);
        }
        break;
      }
      case ResultModifierType::LIMIT_MODIFIER:
        bound_modifier = BindLimit(order_binder, (LimitModifier &)*mod);
        break;
      default:
        throw Exception("Unsupported result modifier");
    }
    if (bound_modifier) {
      result.modifiers_.push_back(move(bound_modifier));
    }
  }
}

static void AssignReturnType(unique_ptr<Expression> &expr, const vector<Type> &sql_types, uint64_t projection_index) {
  if (!expr) {
    return;
  }
  if (expr->type_ != ExpressionType::BOUND_COLUMN_REF) {
    return;
  }
  auto &bound_colref = (BoundColumnRefExpression &)*expr;
  bound_colref.return_type_ = sql_types[bound_colref.binding_.column_index_];
}

void Binder::BindModifierTypes(BoundQueryNode &result, const vector<Type> &sql_types, uint64_t projection_index) {
  for (auto &bound_mod : result.modifiers_) {
    switch (bound_mod->type_) {
      case ResultModifierType::DISTINCT_MODIFIER: {
        auto &distinct = (BoundDistinctModifier &)*bound_mod;
        if (distinct.target_distincts_.empty()) {
          // DISTINCT without a target: push references to the standard select list
          for (uint64_t i = 0; i < sql_types.size(); i++) {
            distinct.target_distincts_.push_back(
                make_unique<BoundColumnRefExpression>(sql_types[i], ColumnBinding(projection_index, i)));
          }
        } else {
          // DISTINCT with target list: set types
          for (auto &expr : distinct.target_distincts_) {
            assert(expr->type_ == ExpressionType::BOUND_COLUMN_REF);
            auto &bound_colref = (BoundColumnRefExpression &)*expr;
            if (bound_colref.binding_.column_index_ == INVALID_INDEX) {
              throw Exception("Ambiguous name in DISTINCT ON!");
            }
            assert(bound_colref.binding_.column_index_ < sql_types.size());
            bound_colref.return_type_ = sql_types[bound_colref.binding_.column_index_];
          }
        }
        // for (auto &target_distinct : distinct.target_distincts_) {
        //   auto &bound_colref = (BoundColumnRefExpression &)*target_distinct;
        //   const auto& sql_type = sql_types[bound_colref.binding_.column_index_];
        //   if (sql_type.GetTypeId() == TypeId::VARCHAR) {
        //     target_distinct = ExpressionBinder::PushCollation(context, move(target_distinct),
        //                                                       StringType::GetCollation(sql_type), true);
        //   }
        // }
        break;
      }
      case ResultModifierType::LIMIT_MODIFIER: {
        auto &limit = (BoundLimitModifier &)*bound_mod;
        AssignReturnType(limit.limit_, sql_types, projection_index);
        AssignReturnType(limit.offset_, sql_types, projection_index);
        break;
      }
      case ResultModifierType::ORDER_MODIFIER: {
        auto &order = (BoundOrderModifier &)*bound_mod;
        for (auto &order_node : order.orders_) {
          auto &expr = order_node.expression_;
          assert(expr->type_ == ExpressionType::BOUND_COLUMN_REF);
          auto &bound_colref = (BoundColumnRefExpression &)*expr;
          if (bound_colref.binding_.column_index_ == INVALID_INDEX) {
            throw Exception("Ambiguous name in ORDER BY!");
          }
          assert(bound_colref.binding_.column_index_ < sql_types.size());
          // auto sql_type = sql_types[bound_colref.binding_.column_index_];
          // bound_colref.return_type = sql_types[bound_colref.binding_.column_index_];
          // if (sql_type.GetTypeId() == TypeId::VARCHAR) {
          //   order_node.expression = ExpressionBinder::PushCollation(context, move(order_node.expression),
          //                                                           StringType::GetCollation(sql_type));
          // }
        }
        break;
      }
      default:
        break;
    }
  }
}

unique_ptr<BoundQueryNode> Binder::BindNode(SelectNode &node) {
  LOG_INFO("In BindNode(SelectNode) for node: %s", node.ToString().c_str());
  auto result = make_unique<BoundSelectNode>();
  result->projection_index_ = GenerateTableIndex();
  result->group_index_ = GenerateTableIndex();
  result->aggregate_index_ = GenerateTableIndex();
  result->groupings_index_ = GenerateTableIndex();
  result->window_index_ = GenerateTableIndex();
  result->unnest_index_ = GenerateTableIndex();
  result->prune_index_ = GenerateTableIndex();

  // first bind the FROM table statement
  result->from_table_ = Bind(*node.from_table_);

  // visit the select list and expand any "*" statements
  vector<unique_ptr<ParsedExpression>> new_select_list;
  for (auto &select_element : node.select_list_) {
    if (select_element->GetExpressionType() == ExpressionType::STAR) {
      // * statement, expand to all columns from the FROM clause
      bind_context_.GenerateAllColumnExpressions((StarExpression &)*select_element, new_select_list);
    } else {
      // regular statement, add it to the list
      new_select_list.push_back(move(select_element));
    }
  }
  if (new_select_list.empty()) {
    throw Exception("SELECT list is empty after resolving * expressions!");
  }
  node.select_list_ = move(new_select_list);

  // create a mapping of (alias -> index) and a mapping of (Expression -> index) for the SELECT list
  case_insensitive_map_t<uint64_t> alias_map;
  expression_map_t<uint64_t> projection_map;
  for (uint64_t i = 0; i < node.select_list_.size(); i++) {
    auto &expr = node.select_list_[i];
    result->names_.push_back(expr->GetName());
    ExpressionBinder::QualifyColumnNames(*this, expr);
    if (!expr->alias_.empty()) {
      alias_map[expr->alias_] = i;
      result->names_[i] = expr->alias_;
    }
    projection_map[expr.get()] = i;
    result->original_expressions_.push_back(expr->Copy());
  }
  result->column_count_ = node.select_list_.size();

  // first visit the WHERE clause
  // the WHERE clause happens before the GROUP BY, PROJECTION or HAVING clauses
  if (node.where_clause_) {
    ColumnAliasBinder alias_binder(*result, alias_map);
    WhereBinder where_binder(*this, context_, &alias_binder);
    unique_ptr<ParsedExpression> condition = move(node.where_clause_);
    result->where_clause_ = where_binder.Bind(condition);
  }

  // now bind all the result modifiers; including DISTINCT and ORDER BY targets
  OrderBinder order_binder({this}, result->projection_index_, node, alias_map, projection_map);
  BindModifiers(order_binder, node, *result);

  vector<unique_ptr<ParsedExpression>> unbound_groups;
  BoundGroupInformation info;
  // auto &group_expressions = node.groups_.group_expressions_;
  // if (!group_expressions.empty()) {
  //   // the statement has a GROUP BY clause, bind it
  //   unbound_groups.resize(group_expressions.size());
  //   GroupBinder group_binder(*this, context_, node, result->group_index_, alias_map, info.alias_map_);
  //   for (uint64_t i = 0; i < group_expressions.size(); i++) {
  //     // we keep a copy of the unbound expression;
  //     // we keep the unbound copy around to check for group references in the SELECT and HAVING clause
  //     // the reason we want the unbound copy is because we want to figure out whether an expression
  //     // is a group reference BEFORE binding in the SELECT/HAVING binder
  //     group_binder.unbound_expression_ = group_expressions[i]->Copy();
  //     group_binder.bind_index_ = i;

  //     // bind the groups
  //     Type group_type = Type(TypeId::INVALID);
  //     auto bound_expr = group_binder.Bind(group_expressions[i], &group_type);
  //     assert(bound_expr->return_type_.GetTypeId() != TypeId::INVALID);

  //     // in the unbound expression we DO bind the table names of any ColumnRefs
  //     // we do this to make sure that "table.a" and "a" are treated the same
  //     // if we wouldn't do this then (SELECT test.a FROM test GROUP BY a) would not work because "test.a" <> "a"
  //     // hence we convert "a" -> "test.a" in the unbound expression
  //     unbound_groups[i] = move(group_binder.unbound_expression_);
  //     ExpressionBinder::QualifyColumnNames(*this, unbound_groups[i]);
  //     info.map_[unbound_groups[i].get()] = i;
  //   }
  // }
  // result->groups_.grouping_sets_ = move(node.groups_.grouping_sets_);

  // bind the HAVING clause, if any
  if (node.having_) {
    HavingBinder having_binder(*this, context_, *result, info, alias_map);
    ExpressionBinder::QualifyColumnNames(*this, node.having_);
    result->having_ = having_binder.Bind(node.having_);
  }

  // after that, we bind to the SELECT list
  SelectBinder select_binder(*this, context_, *result, info);
  vector<Type> internal_sql_types;
  for (uint64_t i = 0; i < node.select_list_.size(); i++) {
    Type result_type = Type(TypeId::INVALID);
    auto expr = select_binder.Bind(node.select_list_[i], &result_type);
    result->select_list_.push_back(move(expr));
    if (i < result->column_count_) {
      result->types_.push_back(result_type);
    }
    internal_sql_types.push_back(result_type);
  }
  result->need_prune_ = result->select_list_.size() > result->column_count_;

  // // now that the SELECT list is bound, we set the types of DISTINCT/ORDER BY expressions
  BindModifierTypes(*result, internal_sql_types, result->projection_index_);
  LOG_INFO("DONE - BindNode(SelectNode) for node: %s", node.ToString().c_str());
  return result;
}

}  // namespace bustub
