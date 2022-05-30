//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_get.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "function/table_function.h"
#include "planner/logical_operator.h"
#include "planner/table_filter.h"

namespace bustub {

//! LogicalGet represents a scan operation from a data source
class LogicalGet : public LogicalOperator {
 public:
  LogicalGet(uint64_t table_index, TableFunction function, unique_ptr<FunctionData> bind_data,
             vector<LogicalType> returned_types, vector<string> returned_names);

  //! The table index in the current bind context
  uint64_t table_index;
  //! The function that is called
  TableFunction function;
  //! The bind data of the function
  unique_ptr<FunctionData> bind_data;
  //! The types of ALL columns that can be returned by the table function
  vector<LogicalType> returned_types;
  //! The names of ALL columns that can be returned by the table function
  vector<string> names;
  //! Bound column IDs
  vector<column_t> column_ids;
  //! Filters pushed down for table scan
  TableFilterSet table_filters;

  string GetName() const override;
  string ParamsToString() const override;
  //! Returns the underlying table that is being scanned, or nullptr if there is none
  TableCatalogEntry *GetTable() const;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

  uint64_t EstimateCardinality(ClientContext &context) override;

 protected:
  void ResolveTypes() override;
};
}  // namespace bustub
