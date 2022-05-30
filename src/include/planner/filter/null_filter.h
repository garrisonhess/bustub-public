//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/filter/null_filter.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/table_filter.h"

namespace bustub {

class IsNullFilter : public TableFilter {
 public:
  IsNullFilter();

 public:
  string ToString(const string &column_name) override;
};

class IsNotNullFilter : public TableFilter {
 public:
  IsNotNullFilter();

 public:
  string ToString(const string &column_name) override;
};

}  // namespace bustub
