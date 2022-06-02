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

namespace bustub {

//! LogicalOrder represents an ORDER BY clause, sorting the data
class LogicalOrder : public LogicalOperator {
 public:
  explicit LogicalOrder(vector<BoundOrderByNode> orders)
      : LogicalOperator(LogicalOperatorType::LOGICAL_ORDER_BY), orders_(move(orders)) {}

  vector<BoundOrderByNode> orders_;

  string ParamsToString() const override {
    string result;
    for (uint64_t i = 0; i < orders_.size(); i++) {
      if (i > 0) {
        result += "\n";
      }
      result += orders_[i].expression_->GetName();
    }
    return result;
  }

 public:
  vector<ColumnBinding> GetColumnBindings() override { return children_[0]->GetColumnBindings(); }

 protected:
  void ResolveTypes() override { types_ = children_[0]->types_; }
};
}  // namespace bustub
