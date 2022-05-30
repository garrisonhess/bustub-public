//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/index_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include "planner/expression_binder.h"

namespace bustub {
class BoundColumnRefExpression;

//! The INDEX binder is responsible for binding an expression within an Index statement
class IndexBinder : public ExpressionBinder {
 public:
  IndexBinder(Binder &binder, ClientContext &context);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  string UnsupportedAggregateMessage() override;
};

}  // namespace bustub
