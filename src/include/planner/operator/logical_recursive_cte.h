//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_recursive_cte.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

class LogicalRecursiveCTE : public LogicalOperator {
 public:
  LogicalRecursiveCTE(uint64_t table_index, uint64_t column_count, bool union_all, unique_ptr<LogicalOperator> top,
                      unique_ptr<LogicalOperator> bottom, LogicalOperatorType type)
      : LogicalOperator(type), union_all(union_all), table_index(table_index), column_count(column_count) {
    D_ASSERT(type == LogicalOperatorType::LOGICAL_RECURSIVE_CTE);
    children.push_back(move(top));
    children.push_back(move(bottom));
  }

  bool union_all;
  uint64_t table_index;
  uint64_t column_count;

 public:
  vector<ColumnBinding> GetColumnBindings() override { return GenerateColumnBindings(table_index, column_count); }

 protected:
  void ResolveTypes() override { types = children[0]->types; }
};
}  // namespace bustub
