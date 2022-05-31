//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/physical_plan_generator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "execution/physical_operator.h"
#include "planner/logical_operator.h"
#include "planner/logical_tokens.h"

namespace duckdb {
class ClientContext;

//! The physical plan generator generates a physical execution plan from a
//! logical query plan
class PhysicalPlanGenerator {
 public:
  explicit PhysicalPlanGenerator(ClientContext &context) : context(context) {}

  unordered_set<CatalogEntry *> dependencies;

 public:
  //! Creates a plan from the logical operator. This involves resolving column bindings and generating physical
  //! operator nodes.
  unique_ptr<PhysicalOperator> CreatePlan(unique_ptr<LogicalOperator> logical);

 protected:
  unique_ptr<PhysicalOperator> CreatePlan(LogicalOperator &op);

  unique_ptr<PhysicalOperator> CreatePlan(LogicalCreateTable &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalCreateIndex &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalDelete &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalDistinct &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalDummyScan &expr);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalEmptyResult &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalExport &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalFilter &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalGet &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalLimit &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalLimitPercent &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalOrder &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalProjection &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalInsert &op);
  unique_ptr<PhysicalOperator> CreatePlan(LogicalUpdate &op);

  unique_ptr<PhysicalOperator> CreateDistinctOn(unique_ptr<PhysicalOperator> child,
                                                vector<unique_ptr<Expression>> distinct_targets);

 private:
  ClientContext &context;
};
}  // namespace duckdb
