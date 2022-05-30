//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_copy_to_file.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/local_file_system.h"
#include "function/copy_function.h"
#include "planner/logical_operator.h"

namespace bustub {

class LogicalCopyToFile : public LogicalOperator {
 public:
  LogicalCopyToFile(CopyFunction function, unique_ptr<FunctionData> bind_data)
      : LogicalOperator(LogicalOperatorType::LOGICAL_COPY_TO_FILE), function(function), bind_data(move(bind_data)) {}
  CopyFunction function;
  unique_ptr<FunctionData> bind_data;
  std::string file_path;
  bool use_tmp_file;
  bool is_file_and_exists;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BIGINT); }
};
}  // namespace bustub
