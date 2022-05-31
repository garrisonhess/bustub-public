#include "planner/table_binding.h"

// #include "catalog/catalog_entry/table_catalog_entry.h"
// #include "catalog/catalog_entry/table_function_catalog_entry.h"
#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "planner/bind_context.h"
#include "planner/bound_query_node.h"
#include "planner/expression/bound_columnref_expression.h"

namespace bustub {

// Binding::Binding(const string &alias, vector<Type> coltypes, vector<string> colnames, uint64_t index)
//     : alias(alias), index(index), types(move(coltypes)), names(move(colnames)) {
//   assert(types.size() == names.size());
//   for (uint64_t i = 0; i < names.size(); i++) {
//     auto &name = names[i];
//     assert(!name.empty());
//     if (name_map.find(name) != name_map.end()) {
//       throw Exception("table has duplicate column name");
//     }
//     name_map[name] = i;
//   }
// }

bool Binding::TryGetBindingIndex(const string &column_name, uint64_t &result) {
  throw bustub::NotImplementedException("");
  //   auto entry = name_map.find(column_name);
  //   if (entry != name_map.end()) {
  //     result = entry->second;
  //     return true;
  //   }
  //   return false;
}

// uint64_t Binding::GetBindingIndex(const string &column_name) {
//   uint64_t result;
//   if (!TryGetBindingIndex(column_name, result)) {
//     throw Exception("Binding index for column not found");
//   }
//   return result;
// }

// bool Binding::HasMatchingBinding(const string &column_name) {
//   uint64_t result;
//   return TryGetBindingIndex(column_name, result);
// }

// string Binding::ColumnNotFoundError(const string &column_name) const {
//   return StringUtil::Format("Values list  does not have a column named ");
// }

// BindResult Binding::Bind(ColumnRefExpression &colref, uint64_t depth) {
//   uint64_t column_index;
//   if (!TryGetBindingIndex(colref.GetColumnName(), column_index)) {
//     return BindResult(ColumnNotFoundError(colref.GetColumnName()));
//   }
//   ColumnBinding binding;
//   binding.table_index = index;
//   binding.column_index = column_index;
//   Type sql_type = types[column_index];
//   if (colref.alias_.empty()) {
//     colref.alias_ = names[column_index];
//   }
//   return BindResult(make_unique<BoundColumnRefExpression>(colref.GetName(), sql_type, binding, depth));
// }

// TableCatalogEntry *Binding::GetTableEntry() { return nullptr; }

// TableBinding::TableBinding(const string &alias, vector<Type> types_p, vector<string> names_p, LogicalGet &get,
//                            uint64_t index, bool add_row_id)
//     : Binding(alias, move(types_p), move(names_p), index), get(get) {
//   if (add_row_id) {
//     if (name_map.find("rowid") == name_map.end()) {
//       name_map["rowid"] = COLUMN_IDENTIFIER_ROW_ID;
//     }
//   }
// }

// BindResult TableBinding::Bind(ColumnRefExpression &colref, uint64_t depth) {
//   auto &column_name = colref.GetColumnName();
//   uint64_t column_index;
//   if (!TryGetBindingIndex(column_name, column_index)) {
//     return BindResult(ColumnNotFoundError(column_name));
//   }
//   // fetch the type of the column
//   Type col_type;
//   if (column_index == COLUMN_IDENTIFIER_ROW_ID) {
//     // row id: BIGINT type
//     col_type = Type(TypeId::BIGINT);
//   } else {
//     // normal column: fetch type from base column
//     col_type = types[column_index];
//     if (colref.alias_.empty()) {
//       colref.alias_ = names[column_index];
//     }
//   }

//   auto &column_ids = get.column_ids;
//   // check if the entry already exists in the column list for the table
//   ColumnBinding binding;

//   binding.column_index = column_ids.size();
//   for (uint64_t i = 0; i < column_ids.size(); i++) {
//     if (column_ids[i] == column_index) {
//       binding.column_index = i;
//       break;
//     }
//   }
//   if (binding.column_index == column_ids.size()) {
//     // column binding not found: add it to the list of bindings
//     column_ids.push_back(column_index);
//   }
//   binding.table_index = index;
//   return BindResult(make_unique<BoundColumnRefExpression>(colref.GetName(), col_type, binding, depth));
// }

// TableCatalogEntry *TableBinding::GetTableEntry() { return get.GetTable(); }

// string TableBinding::ColumnNotFoundError(const string &column_name) const {
//   return StringUtil::Format("Table does not have a column named");
// }

// MacroBinding::MacroBinding(vector<Type> types_p, vector<string> names_p, string macro_name_p)
//     : Binding(MacroBinding::MACRO_NAME, move(types_p), move(names_p), -1), macro_name(move(macro_name_p)) {}

// BindResult MacroBinding::Bind(ColumnRefExpression &colref, uint64_t depth) {
//   uint64_t column_index;
//   if (!TryGetBindingIndex(colref.GetColumnName(), column_index)) {
//     throw Exception("Column not found in macro");
//   }
//   ColumnBinding binding;
//   binding.table_index = index;
//   binding.column_index = column_index;

//   // we are binding a parameter to create the macro, no arguments are supplied
//   return BindResult(make_unique<BoundColumnRefExpression>(colref.GetName(), types[column_index], binding, depth));
// }

// unique_ptr<ParsedExpression> MacroBinding::ParamToArg(ColumnRefExpression &colref) {
//   uint64_t column_index;
//   if (!TryGetBindingIndex(colref.GetColumnName(), column_index)) {
//     throw Exception("Column not found in macro");
//   }
//   auto arg = arguments[column_index]->Copy();
//   arg->alias_ = colref.alias_;
//   return arg;
// }

}  // namespace bustub
