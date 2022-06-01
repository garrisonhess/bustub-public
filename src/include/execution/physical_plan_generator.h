//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/physical_plan_generator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "execution/plans/abstract_plan.h"
#include "planner/logical_operator.h"
#include "planner/logical_tokens.h"

namespace bustub {
class ClientContext;

//! The physical plan generator generates a physical execution plan from a
//! logical query plan
class PhysicalPlanGenerator {
 public:
  explicit PhysicalPlanGenerator(ClientContext &context);
  ~PhysicalPlanGenerator() = default;

 public:
  //! Creates a plan from the logical operator. This involves resolving column bindings and generating physical
  //! operator nodes.
  unique_ptr<AbstractPlanNode> CreatePlan(unique_ptr<LogicalOperator> op);

 protected:
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalOperator &op);

  unique_ptr<AbstractPlanNode> CreatePlan(LogicalCreateTable &op);
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalCreateIndex &op);
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalDelete &op);
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalDistinct &op);
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalDummyScan &expr);
  unique_ptr<AbstractPlanNode> CreatePlan(LogicalEmptyResult &op);

 private:
  ClientContext &context_;
};
}  // namespace bustub
