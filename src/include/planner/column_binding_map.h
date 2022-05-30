//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/column_binding_map.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "common/hash_util.h"
#include "planner/column_binding.h"

namespace bustub {

struct ColumnBindingHashFunction {
  uint64_t operator()(const ColumnBinding &a) const {
    return CombineHash(Hash<uint64_t>(a.table_index), Hash<uint64_t>(a.column_index));
  }
};

struct ColumnBindingEquality {
  bool operator()(const ColumnBinding &a, const ColumnBinding &b) const { return a == b; }
};

template <typename T>
using column_binding_map_t = unordered_map<ColumnBinding, T, ColumnBindingHashFunction, ColumnBindingEquality>;

using column_binding_set_t = unordered_set<ColumnBinding, ColumnBindingHashFunction, ColumnBindingEquality>;

}  // namespace bustub
