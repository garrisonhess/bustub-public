//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_cteref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_tableref.h"

namespace bustub {

class BoundCTERef : public BoundTableRef {
 public:
  BoundCTERef(uint64_t bind_index, uint64_t cte_index)
      : BoundTableRef(TableReferenceType::CTE), bind_index(bind_index), cte_index(cte_index) {}

  //! The set of columns bound to this base table reference
  vector<string> bound_columns;
  //! The types of the values list
  vector<LogicalType> types;
  //! The index in the bind context
  uint64_t bind_index;
  //! The index of the cte
  uint64_t cte_index;
};
}  // namespace bustub
