//===----------------------------------------------------------------------===//
//                         BusTub
//
// main/prepared_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/constants.h"
#include "common/enums/statement_type.h"
#include "main/query_result.h"
#include "parser/sql_statement.h"
#include "planner/plans/abstract_plan.h"
#include "type/type.h"
#include "type/value.h"

namespace bustub {
using std::vector;
class ClientContext;
class AbstractExecutor;
class LogicalOperator;

//! A prepared statement
class PreparedStatement {
 public:
  //! Create a successfully prepared prepared statement object with the given name
  explicit PreparedStatement(shared_ptr<ClientContext> context, string query);

  //! Create a prepared statement that was not successfully prepared
  explicit PreparedStatement(string error);

  ~PreparedStatement() = default;

 public:
  //! The client context this prepared statement belongs to
  shared_ptr<ClientContext> context_;

  //! The query that is being prepared
  string query_;

  //! Whether or not the statement was successfully prepared
  bool success_;

  // DDL executes immediately.
  bool completed_;

  //! The error message (if success = false)
  string error_;

  StatementType statement_type_;

  unique_ptr<AbstractPlanNode> plan_;

  // Output schema
  Schema *schema_;

  //! The result names of the transaction
  vector<string> names_;

  //! The result types of the transaction
  vector<Type> types_;

 public:
  //! Returns the number of columns in the result
  int64_t ColumnCount();

  //! Returns the statement type of the underlying prepared statement object
  StatementType GetStatementType();

  //! Returns the result SQL types of the prepared statement
  const vector<Type> &GetTypes();

  //! Returns the result names of the prepared statement
  const vector<string> &GetNames();

  //! Execute the prepared statement with the given set of values
  unique_ptr<QueryResult> Execute();
};

}  // namespace bustub
