//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_explain.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/statement/explain_statement.h"
#include "planner/logical_operator.h"

namespace bustub {

class LogicalExplain : public LogicalOperator {
 public:
  LogicalExplain(unique_ptr<LogicalOperator> plan, ExplainType explain_type)
      : LogicalOperator(LogicalOperatorType::LOGICAL_EXPLAIN), explain_type(explain_type) {
    children.push_back(move(plan));
  }

  ExplainType explain_type;
  string physical_plan;
  string logical_plan_unopt;
  string logical_plan_opt;

 protected:
  void ResolveTypes() override { types = {LogicalType::VARCHAR, LogicalType::VARCHAR}; }
  vector<ColumnBinding> GetColumnBindings() override { return {ColumnBinding(0, 0), ColumnBinding(0, 1)}; }
};
}  // namespace bustub
