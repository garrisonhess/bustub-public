//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/sql_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/exception.h"
#include "type/statement_type.h"

namespace bustub {

//! SQLStatement is the base class of any type of SQL statement.
class SQLStatement {
 public:
  explicit SQLStatement(StatementType type) : type_(type){};
  virtual ~SQLStatement() = default;

  //! The statement type
  StatementType type_;
  //! The statement location within the query string
  int32_t stmt_location_;
  //! The statement length within the query string
  int32_t stmt_length_;
  //! The number of prepared statement parameters (if any)
  int32_t n_param_;
  //! The query text that corresponds to this SQL statement
  std::string query_;

 protected:
  SQLStatement(const SQLStatement &other) = default;

 public:
  virtual std::string ToString() const { throw Exception("ToString not supported for this type of SQLStatement"); }
  //! Create a copy of this SelectStatement
  virtual std::unique_ptr<SQLStatement> Copy() const = 0;
};
}  // namespace bustub
