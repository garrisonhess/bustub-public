//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/check_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/unordered_set.h"
#include "parser/column_definition.h"
#include "planner/expression_binder.h"

namespace bustub {
//! The CHECK binder is responsible for binding an expression within a CHECK constraint
class CheckBinder : public ExpressionBinder {
 public:
  CheckBinder(Binder &binder, ClientContext &context, string table, vector<ColumnDefinition> &columns,
              unordered_set<column_t> &bound_columns);

  string table;
  vector<ColumnDefinition> &columns;
  unordered_set<column_t> &bound_columns;

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  BindResult BindCheckColumn(ColumnRefExpression &expr);

  string UnsupportedAggregateMessage() override;
};

}  // namespace bustub
