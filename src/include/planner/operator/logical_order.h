//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_order.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_query_node.h"
#include "planner/logical_operator.h"
#include "storage/statistics/base_statistics.h"

namespace bustub {

//! LogicalOrder represents an ORDER BY clause, sorting the data
class LogicalOrder : public LogicalOperator {
 public:
  explicit LogicalOrder(vector<BoundOrderByNode> orders)
      : LogicalOperator(LogicalOperatorType::LOGICAL_ORDER_BY), orders(move(orders)) {}

  vector<BoundOrderByNode> orders;

  string ParamsToString() const override {
    string result;
    for (uint64_t i = 0; i < orders.size(); i++) {
      if (i > 0) {
        result += "\n";
      }
      result += orders[i].expression->GetName();
    }
    return result;
  }

 public:
  vector<ColumnBinding> GetColumnBindings() override { return children[0]->GetColumnBindings(); }

 protected:
  void ResolveTypes() override { types = children[0]->types; }
};
}  // namespace bustub
