//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/returning_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

//! The RETURNING binder is responsible for binding expressions within the RETURNING statement
class ReturningBinder : public ExpressionBinder {
 public:
  ReturningBinder(Binder &binder, ClientContext &context);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;
};

}  // namespace bustub
