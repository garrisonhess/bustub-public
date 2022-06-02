//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/filter/constant_filter.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/expression_type.h"
#include "planner/table_filter.h"
#include "type/value.h"

namespace bustub {

class ConstantFilter : public TableFilter {
 public:
  ConstantFilter(ExpressionType comparison_type, Value constant);

  //! The comparison type (e.g. COMPARE_EQUAL, COMPARE_GREATERTHAN, COMPARE_LESSTHAN, ...)
  ExpressionType comparison_type_;
  //! The constant value to filter on
  Value constant_;

 public:
  string ToString(const string &column_name) override;
  bool Equals(const TableFilter &other) const override;
};

}  // namespace bustub
