//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_table_function.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_tableref.h"
#include "planner/logical_operator.h"

namespace bustub {

//! Represents a reference to a table-producing function call
class BoundTableFunction : public BoundTableRef {
 public:
  explicit BoundTableFunction(unique_ptr<LogicalOperator> get)
      : BoundTableRef(TableReferenceType::TABLE_FUNCTION), get(move(get)) {}

  unique_ptr<LogicalOperator> get;
};

}  // namespace bustub
