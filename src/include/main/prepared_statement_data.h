//===----------------------------------------------------------------------===//
//                         DuckDB
//
// main/prepared_statement_data.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "common/constants.h"
#include "type/statement_type.h"
#include "type/value.h"

namespace bustub {
class SQLStatement;
using std::vector;

class PreparedStatementData {
 public:
  explicit PreparedStatementData(StatementType type);
  ~PreparedStatementData();

  StatementType statement_type_;

  //! The unbound SQL statement that was prepared
  unique_ptr<SQLStatement> unbound_statement_;

  // //! The fully prepared physical plan of the prepared statement
  // unique_ptr<PhysicalOperator> plan_;

  //! The map of parameter index to the actual value entry
  std::unordered_map<int64_t, vector<unique_ptr<Value>>> value_map_;

  //! The result names of the transaction
  vector<string> names_;
  //! The result types of the transaction
  vector<Type> types_;

  //! The statement properties
  StatementProperties properties_;
};

}  // namespace bustub