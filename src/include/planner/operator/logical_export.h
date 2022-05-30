//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_export.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/copy_info.h"
#include "parser/parsed_data/exported_table_data.h"
#include "planner/logical_operator.h"

namespace bustub {

class LogicalExport : public LogicalOperator {
 public:
  LogicalExport(CopyFunction function, unique_ptr<CopyInfo> copy_info, BoundExportData exported_tables)
      : LogicalOperator(LogicalOperatorType::LOGICAL_EXPORT),
        function(function),
        copy_info(move(copy_info)),
        exported_tables(move(exported_tables)) {}
  CopyFunction function;
  unique_ptr<CopyInfo> copy_info;
  BoundExportData exported_tables;

 protected:
  void ResolveTypes() override { types.emplace_back(Type::BOOLEAN); }
};

}  // namespace bustub
