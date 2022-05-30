//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_set.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/set_scope.h"
#include "function/copy_function.h"
#include "parser/parsed_data/copy_info.h"
#include "planner/logical_operator.h"

namespace bustub {

class LogicalSet : public LogicalOperator {
 public:
  LogicalSet(std::string name_p, Value value_p, SetScope scope_p)
      : LogicalOperator(LogicalOperatorType::LOGICAL_SET), name(name_p), value(value_p), scope(scope_p) {}

  std::string name;
  Value value;
  SetScope scope;

 protected:
  void ResolveTypes() override { types.emplace_back(Type::BOOLEAN); }
};

}  // namespace bustub
