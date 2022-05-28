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
// class CatalogEntry;
// class PhysicalOperator;
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

  //! The catalog version of when the prepared statement was bound
  //! If this version is lower than the current catalog version, we have to rebind the prepared statement
  int64_t catalog_version_;

 public:
  //! Bind a set of values to the prepared statement data
  void Bind(vector<Value> values);

  // //! Get the expected SQL Type of the bound parameter
  // Type GetType(int64_t param_index);
};

}  // namespace bustub