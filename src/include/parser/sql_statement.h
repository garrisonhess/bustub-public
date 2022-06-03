//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/sql_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/statement_type.h"
#include "common/exception.h"

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

  //! The query text that corresponds to this SQL statement
  std::string query_;
};

}  // namespace bustub
