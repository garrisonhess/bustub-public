//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder/qualify_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expression_binder/column_alias_binder.h"
#include "planner/expression_binder/select_binder.h"

namespace bustub {

//! The QUALIFY binder is responsible for binding an expression within the QUALIFY clause of a SQL statement
class QualifyBinder : public SelectBinder {
 public:
  QualifyBinder(Binder &binder, ClientContext &context, BoundSelectNode &node, BoundGroupInformation &info,
                case_insensitive_map_t<uint64_t> &alias_map);

 protected:
  BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth,
                            bool root_expression = false) override;

 private:
  BindResult BindColumnRef(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression);

  ColumnAliasBinder column_alias_binder;
};

}  // namespace bustub
