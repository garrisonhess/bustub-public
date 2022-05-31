//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bound_tableref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "common/enums/tableref_type.h"

namespace bustub {

class BoundTableRef {
 public:
  explicit BoundTableRef(TableReferenceType type) : type_(type) {}
  virtual ~BoundTableRef() = default;

  //! The type of table reference
  TableReferenceType type_;
};
}  // namespace bustub
