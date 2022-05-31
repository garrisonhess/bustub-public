//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bound_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"

namespace bustub {

struct BoundStatement {
  unique_ptr<LogicalOperator> plan_;
  vector<Type> types_;
  vector<string> names_;
};

}  // namespace bustub
