//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/where_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

class ColumnAliasBinder;

//! The WHERE binder is responsible for binding an expression within the WHERE clause of a SQL statement
class WhereBinder : public ExpressionBinder {
 public:
  WhereBinder(Binder &binder, ClientContext &context, ColumnAliasBinder *column_alias_binder = nullptr);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  string UnsupportedAggregateMessage() override;

 private:
  BindResult BindColumnRef(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression);

  ColumnAliasBinder *column_alias_binder;
};

}  // namespace bustub
