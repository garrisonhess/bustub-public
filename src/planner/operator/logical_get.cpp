#include "planner/operator/logical_get.h"

// #include "catalog/catalog_entry/table_catalog_entry.h"
#include "storage/data_table.h"
#include "common/string_util.h"
#include "planner/table_binding.h"
#include "function/table/table_scan.h"

namespace bustub {

LogicalGet::LogicalGet(uint64_t table_index, TableFunction function, unique_ptr<FunctionData> bind_data,
                       vector<Type> returned_types, vector<string> returned_names)
    : LogicalOperator(LogicalOperatorType::LOGICAL_GET), table_index_(table_index), function_(move(function)),
      bind_data_(move(bind_data)), returned_types_(move(returned_types)), names_(move(returned_names)) {
}


string LogicalGet::GetName() const { return StringUtil::Upper(function_.name); }

TableCatalogEntry *LogicalGet::GetTable() const { return TableScanFunction::GetTableEntry(function, bind_data.get()); }

string LogicalGet::ParamsToString() const {
  string result;
  for (auto &kv : table_filters.filters) {
    auto &column_index = kv.first;
    auto &filter = kv.second;
    if (column_index < names.size()) {
      result += filter->ToString(names[column_index]);
    }
    result += "\n";
  }
  if (!function_.to_string) {
    return string();
  }
  return function_.to_string(bind_data_.get());
}

vector<ColumnBinding> LogicalGet::GetColumnBindings() {
  if (column_ids.empty()) {
    return {ColumnBinding(table_index, 0)};
  }
  vector<ColumnBinding> result;
  for (idx_t i = 0; i < column_ids.size(); i++) {
    result.emplace_back(table_index, i);
  }
  return result;
}

void LogicalGet::ResolveTypes() {
  if (column_ids.empty()) {
    column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);
  }
  for (auto &index : column_ids) {
    if (index == COLUMN_IDENTIFIER_ROW_ID) {
      types.emplace_back(LogicalType::ROW_TYPE);
    } else {
      types.push_back(returned_types[index]);
    }
  }
}

idx_t LogicalGet::EstimateCardinality(ClientContext &context) {
  if (function.cardinality) {
    auto node_stats = function.cardinality(context, bind_data.get());
    if (node_stats && node_stats->has_estimated_cardinality) {
      return node_stats->estimated_cardinality;
    }
  }
  return 1;
}

}  // namespace bustub
