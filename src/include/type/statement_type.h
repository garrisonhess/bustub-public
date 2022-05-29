//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/common/enums/statement_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <iostream>

namespace bustub {

//===--------------------------------------------------------------------===//
// Statement Types
//===--------------------------------------------------------------------===//
enum class StatementType : uint8_t {
  INVALID_STATEMENT,       // invalid statement type
  SELECT_STATEMENT,        // select statement type
  INSERT_STATEMENT,        // insert statement type
  UPDATE_STATEMENT,        // update statement type
  CREATE_STATEMENT,        // create statement type
  DELETE_STATEMENT,        // delete statement type
  PREPARE_STATEMENT,       // prepare statement type
  EXECUTE_STATEMENT,       // execute statement type
  ALTER_STATEMENT,         // alter statement type
  TRANSACTION_STATEMENT,   // transaction statement type,
  COPY_STATEMENT,          // copy type
  ANALYZE_STATEMENT,       // analyze type
  VARIABLE_SET_STATEMENT,  // variable set statement type
  CREATE_FUNC_STATEMENT,   // create func statement type
  EXPLAIN_STATEMENT,       // explain statement type
  DROP_STATEMENT,          // DROP statement type
  EXPORT_STATEMENT,        // EXPORT statement type
  PRAGMA_STATEMENT,        // PRAGMA statement type
  SHOW_STATEMENT,          // SHOW statement type
  VACUUM_STATEMENT,        // VACUUM statement type
  CALL_STATEMENT,          // CALL statement type
  SET_STATEMENT,           // SET statement type
  LOAD_STATEMENT,          // LOAD statement type
  RELATION_STATEMENT
};

std::string StatementTypeToString(StatementType type);

enum class StatementReturnType : uint8_t {
  QUERY_RESULT,  // the statement returns a query result (e.g. for display to the user)
  CHANGED_ROWS,  // the statement returns a single row containing the number of changed rows (e.g. an insert stmt)
  NOTHING        // the statement returns nothing
};

//! A struct containing various properties of a SQL statement
struct StatementProperties {
  StatementProperties() = default;

  //! What type of data the statement returns
  StatementReturnType return_type_{StatementReturnType::QUERY_RESULT};
};

}  // namespace bustub
