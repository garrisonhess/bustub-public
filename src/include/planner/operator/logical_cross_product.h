//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_cross_product.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

//! LogicalCrossProduct represents a cross product between two relations
class LogicalCrossProduct : public LogicalOperator {
 public:
  LogicalCrossProduct();

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
