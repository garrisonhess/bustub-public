//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_dummytableref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_tableref.h"

namespace bustub {

//! Represents a cross product
class BoundEmptyTableRef : public BoundTableRef {
 public:
  explicit BoundEmptyTableRef(uint64_t bind_index)
      : BoundTableRef(TableReferenceType::EMPTY), bind_index_(bind_index) {}
  uint64_t bind_index_;
};
}  // namespace bustub
