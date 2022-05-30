//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/alter_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/column_definition.h"
#include "planner/expression_binder.h"

namespace bustub {
class TableCatalogEntry;

//! The ALTER binder is responsible for binding an expression within alter statements
class AlterBinder : public ExpressionBinder {
 public:
  AlterBinder(Binder &binder, ClientContext &context, TableCatalogEntry &table, vector<column_t> &bound_columns,
              Type target_type);

  TableCatalogEntry &table;
  vector<column_t> &bound_columns;

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  BindResult BindColumn(ColumnRefExpression &expr);

  string UnsupportedAggregateMessage() override;
};

}  // namespace bustub
