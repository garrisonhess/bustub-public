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
#include "parser/parsed_data/sample_options.h"

namespace bustub {

class BoundTableRef {
 public:
  explicit BoundTableRef(TableReferenceType type) : type(type) {}
  virtual ~BoundTableRef() {}

  //! The type of table reference
  TableReferenceType type;
  //! The sample options (if any)
  unique_ptr<SampleOptions> sample;
};
}  // namespace bustub
