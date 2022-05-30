//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_pragma.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "function/pragma_function.h"
#include "parser/parsed_data/pragma_info.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalSimple represents a simple logical operator that only passes on the parse info
class LogicalPragma : public LogicalOperator {
 public:
  LogicalPragma(PragmaFunction function_p, PragmaInfo info_p)
      : LogicalOperator(LogicalOperatorType::LOGICAL_PRAGMA), function(move(function_p)), info(move(info_p)) {}

  //! The pragma function to call
  PragmaFunction function;
  //! The context of the call
  PragmaInfo info;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BOOLEAN); }
};
}  // namespace bustub
