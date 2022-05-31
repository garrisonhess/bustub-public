//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/constant_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

//! The Constant binder can bind ONLY constant foldable expressions (i.e. no subqueries, column refs, etc)
class ConstantBinder : public ExpressionBinder {
 public:
  ConstantBinder(Binder &binder, ClientContext &context, string clause);

  //! The location where this binder is used, used for error messages
  string clause;

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression) override;
};

}  // namespace bustub
