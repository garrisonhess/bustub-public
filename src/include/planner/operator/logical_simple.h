//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_simple.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/statement_type.h"
#include "parser/parsed_data/parse_info.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalSimple represents a simple logical operator that only passes on the parse info
class LogicalSimple : public LogicalOperator {
 public:
  LogicalSimple(LogicalOperatorType type, unique_ptr<ParseInfo> info) : LogicalOperator(type), info(move(info)) {}

  unique_ptr<ParseInfo> info;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BOOLEAN); }
};
}  // namespace bustub
