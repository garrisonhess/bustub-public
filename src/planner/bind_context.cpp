#include "planner/bind_context.h"

#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/star_expression.h"
#include "planner/bound_query_node.h"
#include "planner/expression/bound_columnref_expression.h"

#include <algorithm>

namespace bustub {

string BindContext::GetMatchingBinding(const string &column_name) {
  string result;
  for (auto &kv : bindings) {
    // auto binding = kv.second.get();
    auto is_using_binding = GetUsingBinding(column_name, kv.first);
    if (is_using_binding != nullptr) {
      continue;
    }
    // if (binding->HasMatchingBinding(column_name)) {
    //   if (!result.empty() || (is_using_binding != nullptr)) {
    //     throw Exception("Ambiguous reference to column name");
    //   }
    //   result = kv.first;
    // }
  }
  return result;
}

void BindContext::AddUsingBinding(const string &column_name, UsingColumnSet *set) {
  using_columns[column_name].insert(set);
}

void BindContext::AddUsingBindingSet(unique_ptr<UsingColumnSet> set) { using_column_sets.push_back(move(set)); }

bool BindContext::FindUsingBinding(const string &column_name, unordered_set<UsingColumnSet *> **out) {
  throw NotImplementedException("");
  // auto entry = using_columns.find(column_name);
  // if (entry != using_columns.end()) {
  //   *out = &entry->second;
  //   return true;
  // }
  // return false;
}

UsingColumnSet *BindContext::GetUsingBinding(const string &column_name) {
  unordered_set<UsingColumnSet *> *using_bindings;
  if (!FindUsingBinding(column_name, &using_bindings)) {
    return nullptr;
  }
  if (using_bindings->size() > 1) {
    string error = "Ambiguous column reference: column \"" + column_name + "\" can refer to either:\n";
    for (auto &using_set : *using_bindings) {
      string result_bindings;
      for (auto &binding : using_set->bindings) {
        if (result_bindings.empty()) {
          result_bindings = "[";
        } else {
          result_bindings += ", ";
        }
        result_bindings += binding;
        result_bindings += ".";
        result_bindings += GetActualColumnName(binding, column_name);
      }
      error += result_bindings + "]";
    }
    throw Exception(error);
  }
  for (auto &using_set : *using_bindings) {
    return using_set;
  }
  throw Exception("Using binding found but no entries");
}

UsingColumnSet *BindContext::GetUsingBinding(const string &column_name, const string &binding_name) {
  if (binding_name.empty()) {
    throw Exception("GetUsingBinding: expected non-empty binding_name");
  }
  unordered_set<UsingColumnSet *> *using_bindings;
  if (!FindUsingBinding(column_name, &using_bindings)) {
    return nullptr;
  }
  for (auto &using_set : *using_bindings) {
    auto &bindings = using_set->bindings;
    if (bindings.find(binding_name) != bindings.end()) {
      return using_set;
    }
  }
  return nullptr;
}

void BindContext::RemoveUsingBinding(const string &column_name, UsingColumnSet *set) {
  if (set == nullptr) {
    return;
  }
  auto entry = using_columns.find(column_name);
  if (entry == using_columns.end()) {
    throw Exception("Attempting to remove using binding that is not there");
  }
  auto &bindings = entry->second;
  if (bindings.find(set) != bindings.end()) {
    bindings.erase(set);
  }
  if (bindings.empty()) {
    using_columns.erase(column_name);
  }
}

void BindContext::TransferUsingBinding(BindContext &current_context, UsingColumnSet *current_set,
                                       UsingColumnSet *new_set, const string &binding, const string &using_column) {
  AddUsingBinding(using_column, new_set);
  current_context.RemoveUsingBinding(using_column, current_set);
}

string BindContext::GetActualColumnName(const string &binding_name, const string &column_name) {
  throw NotImplementedException("");
  // string error;
  // auto binding = GetBinding(binding_name, error);
  // if (binding == nullptr) {
  //   throw Exception("No binding with name ");
  // }
  // uint64_t binding_index;
  // if (!binding->TryGetBindingIndex(column_name, binding_index)) {  // LCOV_EXCL_START
  //   throw Exception("Binding with name  does not have a column named ");
  // }
  // return binding->names[binding_index];
}

unordered_set<string> BindContext::GetMatchingBindings(const string &column_name) {
  unordered_set<string> result;
  // for (auto &kv : bindings) {
  // auto binding = kv.second.get();
  // if (binding->HasMatchingBinding(column_name)) {
  //   result.insert(kv.first);
  // }
  // }
  return result;
}

unique_ptr<ParsedExpression> BindContext::CreateColumnReference(const string &table_name, const string &column_name) {
  string schema_name;
  return CreateColumnReference(schema_name, table_name, column_name);
}

unique_ptr<ParsedExpression> BindContext::CreateColumnReference(const string &schema_name, const string &table_name,
                                                                const string &column_name) {
  string error_message;
  vector<string> names;
  if (!schema_name.empty()) {
    names.push_back(schema_name);
  }
  names.push_back(table_name);
  names.push_back(column_name);

  auto result = make_unique<ColumnRefExpression>(move(names));
  // because of case insensitivity in the binder we rename the column to the original name
  // as it appears in the binding itself
  auto binding = GetBinding(table_name, error_message);
  if (binding != nullptr) {
    // auto column_index = binding->GetBindingIndex(column_name);
    // if (column_index < binding->names.size() && binding->names[column_index] != column_name) {
    //   result->alias_ = binding->names[column_index];
    // }
  }
  return result;
}

Binding *BindContext::GetBinding(const string &name, string &out_error) {
  auto match = bindings.find(name);
  if (match == bindings.end()) {
    // alias not found in this BindContext
    vector<string> candidates;
    for (auto &kv : bindings) {
      candidates.push_back(kv.first);
    }
    // string candidate_str =
    //     StringUtil::CandidatesMessage(StringUtil::TopNLevenshtein(candidates, name), "Candidate tables");
    // out_error = StringUtil::Format("Referenced table \"%s\" not found!%s", name, candidate_str);
    return nullptr;
  }
  return match->second.get();
}

BindResult BindContext::BindColumn(ColumnRefExpression &colref, uint64_t depth) {
  if (!colref.IsQualified()) {
    throw Exception("Could not bind alias!");
  }

  string error;
  auto binding = GetBinding(colref.GetTableName(), error);
  if (binding == nullptr) {
    return BindResult(error);
  }
  return binding->Bind(colref, depth);
}

void BindContext::GenerateAllColumnExpressions(StarExpression &expr,
                                               vector<unique_ptr<ParsedExpression>> &new_select_list) {
  if (bindings_list.empty()) {
    throw Exception("SELECT * expression without FROM clause!");
  }
  case_insensitive_set_t excluded_columns;
  if (expr.relation_name_.empty()) {
    // SELECT * case
    // bind all expressions of each table in-order
    unordered_set<UsingColumnSet *> handled_using_columns;
    for (auto &entry : bindings_list) {
      auto binding = entry.second;
      for (auto &column_name : binding->names) {
        new_select_list.push_back(make_unique<ColumnRefExpression>(column_name, binding->alias));
      }
    }
  } else {
    // SELECT tbl.* case
    string error;
    auto binding = GetBinding(expr.relation_name_, error);
    if (binding == nullptr) {
      throw Exception(error);
    }
    for (auto &column_name : binding->names) {
      new_select_list.push_back(make_unique<ColumnRefExpression>(column_name, binding->alias));
    }
  }
}

void BindContext::AddBinding(const string &alias, unique_ptr<Binding> binding) {
  if (bindings.find(alias) != bindings.end()) {
    throw Exception("Duplicate alias in query!");
  }
  bindings_list.emplace_back(alias, binding.get());
  bindings[alias] = move(binding);
}

void BindContext::AddBaseTable(uint64_t index, const string &alias, const vector<string> &names,
                               const vector<Type> &types, LogicalGet &get) {
  AddBinding(alias, make_unique<TableBinding>(alias, types, names, get, index, true));
}

void BindContext::AddTableFunction(uint64_t index, const string &alias, const vector<string> &names,
                                   const vector<Type> &types, LogicalGet &get) {
  AddBinding(alias, make_unique<TableBinding>(alias, types, names, get, index));
}

static string AddColumnNameToBinding(const string &base_name, case_insensitive_set_t &current_names) {
  uint64_t index = 1;
  string name = base_name;
  while (current_names.find(name) != current_names.end()) {
    name = base_name + ":" + std::to_string(index++);
  }
  current_names.insert(name);
  return name;
}

vector<string> BindContext::AliasColumnNames(const string &table_name, const vector<string> &names,
                                             const vector<string> &column_aliases) {
  vector<string> result;
  if (column_aliases.size() > names.size()) {
    throw Exception("table X has columns available but Y columns specified");
  }
  case_insensitive_set_t current_names;
  // use any provided column aliases first
  for (const auto &column_aliase : column_aliases) {
    result.push_back(AddColumnNameToBinding(column_aliase, current_names));
  }
  // if not enough aliases were provided, use the default names for remaining columns
  for (uint64_t i = column_aliases.size(); i < names.size(); i++) {
    result.push_back(AddColumnNameToBinding(names[i], current_names));
  }
  return result;
}

void BindContext::AddGenericBinding(uint64_t index, const string &alias, const vector<string> &names,
                                    const vector<Type> &types) {
  AddBinding(alias, make_unique<Binding>(alias, types, names, index));
}

void BindContext::AddContext(BindContext other) {
  for (auto &binding : other.bindings) {
    if (bindings.find(binding.first) != bindings.end()) {
      throw Exception("Duplicate alias in query!");
    }
    bindings[binding.first] = move(binding.second);
  }
  for (auto &binding : other.bindings_list) {
    bindings_list.push_back(move(binding));
  }
  for (auto &entry : other.using_columns) {
    for (auto &alias : entry.second) {
      using_columns[entry.first].insert(alias);
    }
  }
}

}  // namespace bustub
