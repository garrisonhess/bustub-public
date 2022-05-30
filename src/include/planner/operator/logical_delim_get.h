//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_delim_get.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalDelimGet represents a duplicate eliminated scan belonging to a DelimJoin
class LogicalDelimGet : public LogicalOperator {
 public:
  LogicalDelimGet(uint64_t table_index, vector<Type> types)
      : LogicalOperator(LogicalOperatorType::LOGICAL_DELIM_GET), table_index(table_index) {
    assert(types.size() > 0);
    chunk_types = types;
  }

  //! The table index in the current bind context
  uint64_t table_index;
  //! The types of the chunk
  vector<Type> chunk_types;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return GenerateColumnBindings(table_index, chunk_types.size()); }

 protected:
  void ResolveTypes() override {
    // types are resolved in the constructor
    this->types = chunk_types;
  }
};
}  // namespace bustub
