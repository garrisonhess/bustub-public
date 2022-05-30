//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_prepare.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include "common/unordered_set.h"
#include "main/prepared_statement_data.h"
#include "planner/logical_operator.h"

namespace bustub {

class TableCatalogEntry;

class LogicalPrepare : public LogicalOperator {
 public:
  LogicalPrepare(string name, shared_ptr<PreparedStatementData> prepared, unique_ptr<LogicalOperator> logical_plan)
      : LogicalOperator(LogicalOperatorType::LOGICAL_PREPARE), name(name), prepared(move(prepared)) {
    children.push_back(move(logical_plan));
  }

  string name;
  shared_ptr<PreparedStatementData> prepared;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BOOLEAN); }
};
}  // namespace bustub
