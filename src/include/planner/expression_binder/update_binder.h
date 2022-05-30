//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/update_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

//! The UPDATE binder is responsible for binding an expression within an UPDATE statement
class UpdateBinder : public ExpressionBinder {
 public:
  UpdateBinder(Binder &binder, ClientContext &context);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  string UnsupportedAggregateMessage() override;
};

}  // namespace bustub
