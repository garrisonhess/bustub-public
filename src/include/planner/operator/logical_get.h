//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_get.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include "planner/logical_operator.h"
#include "planner/table_binding.h"
#include "planner/table_filter.h"

namespace bustub {

//! LogicalGet represents a scan operation from a data source
class LogicalGet : public LogicalOperator {
 public:
  explicit LogicalGet(uint64_t table_index, vector<Type> returned_types, vector<string> returned_names);

  //! The table index in the current bind context
  uint64_t table_index_;
  //! The types of ALL columns that can be returned by the table function
  vector<Type> returned_types_;
  //! The names of ALL columns that can be returned by the table function
  vector<string> names_;
  //! Bound column IDs
  vector<uint64_t> column_ids_;
  //! Filters pushed down for table scan
  TableFilterSet table_filters_;

  string GetName() const override;
  string ParamsToString() const override;
  //! Returns the underlying table that is being scanned, or nullptr if there is none
  TableCatalogEntry *GetTable() const;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
