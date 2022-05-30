//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/order_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
#include "parser/expression_map.h"
#include "parser/parsed_expression.h"
#include "planner/expression/bound_columnref_expression.h"

namespace bustub {
class Binder;
class Expression;
class SelectNode;

//! The ORDER binder is responsible for binding an expression within the ORDER BY clause of a SQL statement
class OrderBinder {
 public:
  OrderBinder(vector<Binder *> binders, uint64_t projection_index, case_insensitive_map_t<uint64_t> &alias_map,
              expression_map_t<uint64_t> &projection_map, uint64_t max_count);
  OrderBinder(vector<Binder *> binders, uint64_t projection_index, SelectNode &node,
              case_insensitive_map_t<uint64_t> &alias_map, expression_map_t<uint64_t> &projection_map);

 public:
  unique_ptr<Expression> Bind(unique_ptr<ParsedExpression> expr);

  uint64_t MaxCount() { return max_count; }

  unique_ptr<Expression> CreateExtraReference(unique_ptr<ParsedExpression> expr);

 private:
  unique_ptr<Expression> CreateProjectionReference(ParsedExpression &expr, uint64_t index);

 private:
  vector<Binder *> binders;
  uint64_t projection_index;
  uint64_t max_count;
  vector<unique_ptr<ParsedExpression>> *extra_list;
  case_insensitive_map_t<uint64_t> &alias_map;
  expression_map_t<uint64_t> &projection_map;
};

}  // namespace bustub
