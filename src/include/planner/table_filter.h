//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/table_filter.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "common/common.h"
#include "common/enums/filter_propagate_result.h"
#include "type/type.h"
namespace bustub {

enum class TableFilterType : uint8_t {
  CONSTANT_COMPARISON = 0,  // constant comparison (e.g. =C, >C, >=C, <C, <=C)
  IS_NULL = 1,
  IS_NOT_NULL = 2,
  CONJUNCTION_OR = 3,
  CONJUNCTION_AND = 4
};

//! TableFilter represents a filter pushed down into the table scan.
class TableFilter {
 public:
  TableFilter(TableFilterType filter_type_p) : filter_type(filter_type_p) {}
  virtual ~TableFilter() {}

  TableFilterType filter_type;

 public:
  virtual string ToString(const string &column_name) = 0;
  virtual bool Equals(const TableFilter &other) const { return filter_type != other.filter_type; }
};

class TableFilterSet {
 public:
  unordered_map<uint64_t, unique_ptr<TableFilter>> filters;

 public:
  void PushFilter(uint64_t table_index, unique_ptr<TableFilter> filter);

  bool Equals(TableFilterSet &other) {
    if (filters.size() != other.filters.size()) {
      return false;
    }
    for (auto &entry : filters) {
      auto other_entry = other.filters.find(entry.first);
      if (other_entry == other.filters.end()) {
        return false;
      }
      if (!entry.second->Equals(*other_entry->second)) {
        return false;
      }
    }
    return true;
  }
  static bool Equals(TableFilterSet *left, TableFilterSet *right) {
    if (left == right) {
      return true;
    }
    if (!left || !right) {
      return false;
    }
    return left->Equals(*right);
  }
};

}  // namespace bustub
