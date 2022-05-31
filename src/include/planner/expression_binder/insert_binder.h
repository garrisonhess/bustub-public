//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/insert_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

//! The INSERT binder is responsible for binding expressions within the VALUES of an INSERT statement
class InsertBinder : public ExpressionBinder {
 public:
  InsertBinder(Binder &binder, ClientContext &context);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression) override;
};

}  // namespace bustub
