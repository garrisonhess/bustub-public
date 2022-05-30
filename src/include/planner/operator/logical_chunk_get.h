//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_chunk_get.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/types/chunk_collection.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalChunkGet represents a scan operation from a ChunkCollection
class LogicalChunkGet : public LogicalOperator {
 public:
  LogicalChunkGet(uint64_t table_index, vector<Type> types, unique_ptr<ChunkCollection> collection)
      : LogicalOperator(LogicalOperatorType::LOGICAL_CHUNK_GET),
        table_index(table_index),
        collection(move(collection)) {
    D_ASSERT(types.size() > 0);
    chunk_types = types;
  }

  //! The table index in the current bind context
  uint64_t table_index;
  //! The types of the chunk
  vector<Type> chunk_types;
  //! The chunk collection to scan
  unique_ptr<ChunkCollection> collection;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return GenerateColumnBindings(table_index, chunk_types.size()); }

 protected:
  void ResolveTypes() override {
    // types are resolved in the constructor
    this->types = chunk_types;
  }
};
}  // namespace bustub
