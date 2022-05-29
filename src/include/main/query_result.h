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
#include <utility>
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
 public:
  explicit QueryResult(StatementType statement_type, vector<Type> types, vector<string> names);
  explicit QueryResult(string error);
  ~QueryResult() = default;

  //! The type of the statement that created this result
  StatementType statement_type_;

  //! The SQL types of the result
  vector<Type> types_;

  //! The names of the result
  vector<string> names_;

  vector<Tuple> data_;
  int curr_idx_;
  bool success_;
  string error_;
};

}  // namespace bustub
