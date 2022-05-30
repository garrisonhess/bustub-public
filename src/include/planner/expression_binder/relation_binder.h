//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/relation_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder.h"

namespace bustub {

//! The relation binder is a binder used to bind expressions in the relation API
class RelationBinder : public ExpressionBinder {
 public:
  RelationBinder(Binder &binder, ClientContext &context, string op);

  string op;

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

  string UnsupportedAggregateMessage() override;
};

}  // namespace bustub
