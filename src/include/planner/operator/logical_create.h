//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_create.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/create_info.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalCreate represents a CREATE operator
class LogicalCreate : public LogicalOperator {
 public:
  LogicalCreate(LogicalOperatorType type, unique_ptr<CreateInfo> info, SchemaCatalogEntry *schema = nullptr)
      : LogicalOperator(type), schema(schema), info(move(info)) {}

  SchemaCatalogEntry *schema;
  unique_ptr<CreateInfo> info;

 protected:
  void ResolveTypes() override { types.emplace_back(LogicalType::BIGINT); }
};
}  // namespace bustub
