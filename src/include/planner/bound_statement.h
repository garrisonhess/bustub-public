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
  unique_ptr<LogicalOperator> plan;
  vector<LogicalType> types;
  vector<string> names;
};

}  // namespace bustub
