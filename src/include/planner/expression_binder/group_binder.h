//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/group_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
#include "planner/expression_binder.h"

namespace bustub {
class ConstantExpression;
class ColumnRefExpression;

//! The GROUP binder is responsible for binding expressions in the GROUP BY clause
class GroupBinder : public ExpressionBinder {
 public:
  GroupBinder(Binder &binder, ClientContext &context, SelectNode &node, uint64_t group_index,
              case_insensitive_map_t<uint64_t> &alias_map, case_insensitive_map_t<uint64_t> &group_alias_map);

  //! The unbound root expression
  unique_ptr<ParsedExpression> unbound_expression;
  //! The group index currently being bound
  uint64_t bind_index;

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) override;

  BindResult BindSelectRef(uint64_t entry);
  BindResult BindColumnRef(ColumnRefExpression &expr);
  BindResult BindConstant(ConstantExpression &expr);

  SelectNode &node;
  case_insensitive_map_t<uint64_t> &alias_map;
  case_insensitive_map_t<uint64_t> &group_alias_map;
  unordered_set<uint64_t> used_aliases;

  uint64_t group_index;
};

}  // namespace bustub
