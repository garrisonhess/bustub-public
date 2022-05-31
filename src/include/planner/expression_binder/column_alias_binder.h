//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/column_alias_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
#include "planner/expression_binder.h"

namespace bustub {

class BoundSelectNode;
class ColumnRefExpression;

//! A helper binder for WhereBinder and HavingBinder which support alias as a columnref.
class ColumnAliasBinder {
 public:
  ColumnAliasBinder(BoundSelectNode &node, const case_insensitive_map_t<uint64_t> &alias_map);

  BindResult BindAlias(ExpressionBinder &enclosing_binder, ColumnRefExpression &expr, uint64_t depth,
                       bool root_expression);

 private:
  BoundSelectNode &node_;
  const case_insensitive_map_t<uint64_t> &alias_map_;
  bool in_alias_;
};

}  // namespace bustub
