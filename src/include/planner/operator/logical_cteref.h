//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_cteref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/types/chunk_collection.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalChunkGet represents a scan operation from a ChunkCollection
class LogicalCTERef : public LogicalOperator {
 public:
  LogicalCTERef(uint64_t table_index, uint64_t cte_index, vector<Type> types, vector<string> colnames)
      : LogicalOperator(LogicalOperatorType::LOGICAL_CTE_REF), table_index(table_index), cte_index(cte_index) {
    assert(types.size() > 0);
    chunk_types = types;
    bound_columns = colnames;
  }

  vector<string> bound_columns;
  //! The table index in the current bind context
  uint64_t table_index;
  //! CTE index
  uint64_t cte_index;
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
