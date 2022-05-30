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

  unique_ptr<LogicalOperator> plan;
  vector<string> names;
  vector<Type> types;
  unordered_map<uint64_t, vector<unique_ptr<Value>>> value_map;
  vector<Type> parameter_types;

  shared_ptr<Binder> binder;
  ClientContext &context;

  StatementProperties properties;

 private:
  void CreatePlan(SQLStatement &statement);
  shared_ptr<PreparedStatementData> PrepareSQLStatement(unique_ptr<SQLStatement> statement);
  void PlanPrepare(unique_ptr<SQLStatement> statement);
  void PlanExecute(unique_ptr<SQLStatement> statement);
};
}  // namespace bustub
