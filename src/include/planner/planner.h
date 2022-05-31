//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/planner.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/sql_statement.h"
#include "planner/binder.h"
#include "planner/logical_operator.h"

namespace bustub {
class ClientContext;
class PreparedStatementData;

//! The planner creates a logical query plan from the parsed SQL statements
//! using the Binder and LogicalPlanGenerator.
class Planner {
 public:
  explicit Planner(ClientContext &context);

  void CreatePlan(unique_ptr<SQLStatement> statement);

  unique_ptr<LogicalOperator> plan_;
  vector<string> names_;
  vector<Type> types_;
  shared_ptr<Binder> binder_;
  ClientContext &context_;

 private:
  void CreatePlan(SQLStatement &statement);
  shared_ptr<PreparedStatementData> PrepareSQLStatement(unique_ptr<SQLStatement> statement);
  // void PlanPrepare(unique_ptr<SQLStatement> statement);
  // void PlanExecute(unique_ptr<SQLStatement> statement);
};
}  // namespace bustub
