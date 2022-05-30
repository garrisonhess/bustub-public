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
  uint64_t table_index;
  uint64_t column_index;

  ColumnBinding() : table_index(DConstants::INVALID_INDEX), column_index(DConstants::INVALID_INDEX) {}
  ColumnBinding(uint64_t table, uint64_t column) : table_index(table), column_index(column) {}

  bool operator==(const ColumnBinding &rhs) const {
    return table_index == rhs.table_index && column_index == rhs.column_index;
  }
};

}  // namespace bustub
