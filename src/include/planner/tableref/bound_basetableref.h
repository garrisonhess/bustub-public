//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/tableref/bound_basetableref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/bound_tableref.h"
#include "planner/logical_operator.h"

namespace bustub {
class TableCatalogEntry;

//! Represents a TableReference to a base table in the schema
class BoundBaseTableRef : public BoundTableRef {
 public:
  BoundBaseTableRef(TableCatalogEntry *table, unique_ptr<LogicalOperator> get)
      : BoundTableRef(TableReferenceType::BASE_TABLE), table(table), get(move(get)) {}

  TableCatalogEntry *table;
  unique_ptr<LogicalOperator> get;
};
}  // namespace bustub
