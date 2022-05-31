//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/column_binding.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <functional>
#include "common/common.h"

namespace bustub {

struct ColumnBinding {
  uint64_t table_index_;
  uint64_t column_index_;

  ColumnBinding() : table_index_(INVALID_INDEX), column_index_(INVALID_INDEX) {}
  ColumnBinding(uint64_t table, uint64_t column) : table_index_(table), column_index_(column) {}

  bool operator==(const ColumnBinding &rhs) const {
    return table_index_ == rhs.table_index_ && column_index_ == rhs.column_index_;
  }
};

}  // namespace bustub
