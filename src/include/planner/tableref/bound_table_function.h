//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/tableref/bound_table_function.hpp
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
      : BoundTableRef(TableReferenceType::TABLE_FUNCTION), get_(move(get)) {}

  unique_ptr<LogicalOperator> get_;
};

}  // namespace bustub
