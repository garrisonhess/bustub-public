//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/parsed_data/bound_create_table_info.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
// #include "parser/parsed_data/create_table_info.h"
#include "planner/expression.h"
#include "planner/logical_operator.h"
#include "storage/table/persistent_table_data.h"

namespace bustub {
class CatalogEntry;

struct BoundCreateTableInfo {
  explicit BoundCreateTableInfo(unique_ptr<CreateInfo> base_p) : base(move(base_p)) { assert(base); }

  //! The schema to create the table in
  SchemaCatalogEntry *schema;
  //! The base CreateInfo object
  unique_ptr<CreateInfo> base;
  //! The map of column names -> column index, used during binding
  case_insensitive_map_t<uint64_t> name_map;
  //! Bound default values
  vector<unique_ptr<Expression>> bound_defaults;
  //! Dependents of the table (in e.g. default values)
  unordered_set<CatalogEntry *> dependencies;
  //! The existing table data on disk (if any)
  unique_ptr<PersistentTableData> data;
  //! CREATE TABLE from QUERY
  unique_ptr<LogicalOperator> query;

  CreateTableInfo &Base() {
    assert(base);
    return (CreateTableInfo &)*base;
  }
};

}  // namespace bustub
