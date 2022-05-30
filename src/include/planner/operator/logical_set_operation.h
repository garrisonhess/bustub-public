//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_set_operation.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

class LogicalSetOperation : public LogicalOperator {
 public:
  LogicalSetOperation(uint64_t table_index, uint64_t column_count, unique_ptr<LogicalOperator> top,
                      unique_ptr<LogicalOperator> bottom, LogicalOperatorType type)
      : LogicalOperator(type), table_index(table_index), column_count(column_count) {
    assert(type == LogicalOperatorType::LOGICAL_UNION || type == LogicalOperatorType::LOGICAL_EXCEPT ||
           type == LogicalOperatorType::LOGICAL_INTERSECT);
    children.push_back(move(top));
    children.push_back(move(bottom));
  }

  uint64_t table_index;
  uint64_t column_count;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return GenerateColumnBindings(table_index, column_count); }

 protected:
  void ResolveTypes() override { types = children[0]->types; }
};
}  // namespace bustub
