//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_create_table.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/logical_operator.h"
#include "planner/parsed_data/bound_create_table_info.h"

namespace bustub {

class LogicalCreateTable : public LogicalOperator {
 public:
  LogicalCreateTable(SchemaCatalogEntry *schema, unique_ptr<BoundCreateTableInfo> info)
      : LogicalOperator(LogicalOperatorType::LOGICAL_CREATE_TABLE), schema(schema), info(move(info)) {}

  //! Schema to insert to
  SchemaCatalogEntry *schema;
  //! Create Table information
  unique_ptr<BoundCreateTableInfo> info;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BIGINT); }
};
}  // namespace bustub
