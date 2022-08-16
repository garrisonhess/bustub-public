//===----------------------------------------------------------------------===//
//                         BusTub
//
// main/query_result.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/statement_type.h"
#include "storage/table/tuple.h"
#include "type/type.h"

#include <string>
#include <vector>

namespace bustub {
using std::vector;

//! The QueryResult object holds the result of a query.
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
