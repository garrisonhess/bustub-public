//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/select_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
#include "parser/expression_map.h"
#include "planner/expression_binder.h"

namespace bustub {
class BoundColumnRefExpression;
class WindowExpression;

class BoundSelectNode;

struct BoundGroupInformation {
  expression_map_t<uint64_t> map_;
  case_insensitive_map_t<uint64_t> alias_map_;
};

//! The SELECT binder is responsible for binding an expression within the SELECT clause of a SQL statement
class SelectBinder : public ExpressionBinder {
 public:
  SelectBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info);

  bool BoundAggregates() { return bound_aggregate_; }
  void ResetBindings() {
    this->bound_aggregate_ = false;
    this->bound_columns_.clear();
  }

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) override;

  // BindResult BindAggregate(FunctionExpression &expr, AggregateFunctionCatalogEntry *function, uint64_t depth)
  // override;

  bool inside_window_;
  bool bound_aggregate_ = false;

  BoundSelectNode &node_;
  BoundGroupInformation &info_;

 protected:
  uint64_t TryBindGroup(ParsedExpression &expr, uint64_t depth);
  BindResult BindGroup(ParsedExpression &expr, uint64_t depth, uint64_t group_index);
};

}  // namespace bustub
