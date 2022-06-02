#include "planner/table_binding.h"

#include <utility>

#include "planner/operator/logical_get.h"
// #include "catalog/catalog_entry/table_catalog_entry.h"
// #include "catalog/catalog_entry/table_function_catalog_entry.h"
// #include "parser/tableref/subqueryref.h"
#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "planner/bind_context.h"
#include "planner/bound_query_node.h"
#include "planner/expression/bound_columnref_expression.h"

namespace bustub {

Binding::Binding(string alias, vector<Type> coltypes, vector<string> colnames, uint64_t index)
    : alias_(std::move(alias)), index_(index), types_(move(coltypes)), names_(move(colnames)) {
  assert(types_.size() == names_.size());
  for (uint64_t i = 0; i < names_.size(); i++) {
    auto &name = names_[i];
    assert(!name.empty());
    if (name_map_.find(name) != name_map_.end()) {
      throw Exception("table has duplicate column name");
    }
    name_map_[name] = i;
  }
}

bool Binding::TryGetBindingIndex(const string &column_name, uint64_t &result) {
  auto entry = name_map_.find(column_name);
  if (entry != name_map_.end()) {
    result = entry->second;
    return true;
  }
  return false;
}

uint64_t Binding::GetBindingIndex(const string &column_name) {
  uint64_t result;
  if (!TryGetBindingIndex(column_name, result)) {
    throw Exception("Binding index for column not found");
  }
  return result;
}

bool Binding::HasMatchingBinding(const string &column_name) {
  uint64_t result;
  return TryGetBindingIndex(column_name, result);
}

string Binding::ColumnNotFoundError(const string &column_name) const {
  return StringUtil::Format("Values list  does not have a column named ");
}

BindResult Binding::Bind(ColumnRefExpression &colref, uint64_t depth) {
  uint64_t column_index;
  if (!TryGetBindingIndex(colref.GetColumnName(), column_index)) {
    return BindResult(ColumnNotFoundError(colref.GetColumnName()));
  }
  ColumnBinding binding;
  binding.table_index_ = index_;
  binding.column_index_ = column_index;
  Type sql_type = types_[column_index];
  if (colref.alias_.empty()) {
    colref.alias_ = names_[column_index];
  }
  return BindResult(make_unique<BoundColumnRefExpression>(colref.GetName(), sql_type, binding, depth));
}

TableCatalogEntry *Binding::GetTableEntry() { return nullptr; }

TableBinding::TableBinding(const string &alias, vector<Type> types_p, vector<string> names_p, LogicalGet &get,
                           uint64_t index, bool add_row_id)
    : Binding(alias, move(types_p), move(names_p), index), get_(get) {
  if (add_row_id) {
    if (name_map_.find("rowid") == name_map_.end()) {
      name_map_["rowid"] = COLUMN_IDENTIFIER_ROW_ID;
    }
  }
}

BindResult TableBinding::Bind(ColumnRefExpression &colref, uint64_t depth) {
  auto &column_name = colref.GetColumnName();
  uint64_t column_index;
  if (!TryGetBindingIndex(column_name, column_index)) {
    return BindResult(ColumnNotFoundError(column_name));
  }
  // fetch the type of the column
  Type col_type = Type(TypeId::INVALID);
  if (column_index == COLUMN_IDENTIFIER_ROW_ID) {
    // row id: BIGINT type
    col_type = Type(TypeId::BIGINT);
  } else {
    // normal column: fetch type from base column
    col_type = types_[column_index];
    if (colref.alias_.empty()) {
      colref.alias_ = names_[column_index];
    }
  }

  auto &column_ids = get_.column_ids_;

  // check if the entry already exists in the column list for the table
  ColumnBinding binding;

  binding.column_index_ = column_ids.size();
  for (uint64_t i = 0; i < column_ids.size(); i++) {
    if (column_ids[i] == column_index) {
      binding.column_index_ = i;
      break;
    }
  }
  if (binding.column_index_ == column_ids.size()) {
    // column binding not found: add it to the list of bindings
    column_ids.push_back(column_index);
  }
  binding.table_index_ = index_;
  return BindResult(make_unique<BoundColumnRefExpression>(colref.GetName(), col_type, binding, depth));
}

TableCatalogEntry *TableBinding::GetTableEntry() {
  throw bustub::NotImplementedException("");
  return get_.GetTable();
}

string TableBinding::ColumnNotFoundError(const string &column_name) const {
  return StringUtil::Format("Table does not have a column named");
}

}  // namespace bustub
