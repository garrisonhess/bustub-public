//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/main/query_result.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "storage/table/tuple.h"
#include "type/data_chunk.h"
#include "type/statement_type.h"
#include "type/type.h"

// struct ArrowSchema;

namespace bustub {
using std::string;
using std::unique_ptr;
using std::vector;

enum class QueryResultType : uint8_t { MATERIALIZED_RESULT, STREAM_RESULT };

//! The QueryResult object holds the result of a query. It can either be a MaterializedQueryResult, in which case the
//! result contains the entire result set, or a StreamQueryResult in which case the Fetch method can be called to
//! incrementally fetch data from the database.
class QueryResult {
  vector<Tuple> data_;
};

}  // namespace bustub
