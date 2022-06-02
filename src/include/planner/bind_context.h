//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bind_context.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "catalog/catalog.h"
// #include "catalog/catalog_entry/table_catalog_entry.h"
// #include "catalog/catalog_entry/table_function_catalog_entry.h"
#include "common/case_insensitive_map.h"
#include "parser/expression/columnref_expression.h"
#include "parser/parsed_expression.h"
#include "planner/expression.h"
#include "planner/expression_binder.h"
#include "planner/table_binding.h"

namespace bustub {
class Binder;
class LogicalGet;
class BoundQueryNode;

class StarExpression;

struct UsingColumnSet {
  string primary_binding_;
  unordered_set<string> bindings_;
};

//! The BindContext object keeps track of all the tables and columns that are
//! encountered during the binding process.
class BindContext {
 public:
  //! Given a column name, find the matching table it belongs to. Throws an
  //! exception if no table has a column of the given name.
  string GetMatchingBinding(const string &column_name);
  //! Like GetMatchingBinding, but instead of throwing an error if multiple tables have the same binding it will
  //! return a list of all the matching ones
  unordered_set<string> GetMatchingBindings(const string &column_name);
  //! Like GetMatchingBindings, but returns the top 3 most similar bindings (in levenshtein distance) instead of the
  //! matching ones
  vector<string> GetSimilarBindings(const string &column_name);

  //! Binds a column expression to the base table. Returns the bound expression
  //! or throws an exception if the column could not be bound.
  BindResult BindColumn(ColumnRefExpression &colref, uint64_t depth);

  unique_ptr<ParsedExpression> CreateColumnReference(const string &table_name, const string &column_name);
  unique_ptr<ParsedExpression> CreateColumnReference(const string &schema_name, const string &table_name,
                                                     const string &column_name);

  //! Generate column expressions for all columns that are present in the
  //! referenced tables. This is used to resolve the * expression in a
  //! selection list.
  void GenerateAllColumnExpressions(StarExpression &expr, vector<unique_ptr<ParsedExpression>> &new_select_list);

  const vector<std::pair<string, Binding *>> &GetBindingsList() { return bindings_list_; }

  //! Adds a base table with the given alias to the BindContext.
  void AddBaseTable(uint64_t index, const string &alias, const vector<string> &names, const vector<Type> &types,
                    LogicalGet &get);
  //! Adds a call to a table function with the given alias to the BindContext.
  void AddTableFunction(uint64_t index, const string &alias, const vector<string> &names, const vector<Type> &types,
                        LogicalGet &get);

  //! Adds a base table with the given alias to the BindContext.
  void AddGenericBinding(uint64_t index, const string &alias, const vector<string> &names, const vector<Type> &types);

  //! Add an implicit join condition (e.g. USING (x))
  void AddUsingBinding(const string &column_name, UsingColumnSet *set);

  void AddUsingBindingSet(unique_ptr<UsingColumnSet> set);

  //! Returns any using column set for the given column name, or nullptr if there is none. On conflict (multiple using
  //! column sets with the same name) throw an exception.
  UsingColumnSet *GetUsingBinding(const string &column_name);

  //! Returns any using column set for the given column name, or nullptr if there is none
  UsingColumnSet *GetUsingBinding(const string &column_name, const string &binding_name);
  //! Erase a using binding from the set of using bindings
  void RemoveUsingBinding(const string &column_name, UsingColumnSet *set);
  //! Finds the using bindings for a given column. Returns true if any exists, false otherwise.
  bool FindUsingBinding(const string &column_name, unordered_set<UsingColumnSet *> **using_columns);
  //! Transfer a using binding from one bind context to this bind context
  void TransferUsingBinding(BindContext &current_context, UsingColumnSet *current_set, UsingColumnSet *new_set,
                            const string &binding, const string &using_column);

  //! Fetch the actual column name from the given binding, or throws if none exists
  //! This can be different from "column_name" because of case insensitivity
  //! (e.g. "column_name" might return "COLUMN_NAME")
  string GetActualColumnName(const string &binding, const string &column_name);

  //! Alias a set of column names for the specified table, using the original names if there are not enough aliases
  //! specified.
  static vector<string> AliasColumnNames(const string &table_name, const vector<string> &names,
                                         const vector<string> &column_aliases);

  //! Add all the bindings from a BindContext to this BindContext. The other BindContext is destroyed in the process.
  void AddContext(BindContext other);

  //! Gets a binding of the specified name. Returns a nullptr and sets the out_error if the binding could not be
  //! found.
  Binding *GetBinding(const string &name, string &out_error);

 private:
  void AddBinding(const string &alias, unique_ptr<Binding> binding);

 private:
  //! The set of bindings
  case_insensitive_map_t<unique_ptr<Binding>> bindings_;
  //! The list of bindings in insertion order
  vector<std::pair<string, Binding *>> bindings_list_;
  //! The set of columns used in USING join conditions
  case_insensitive_map_t<unordered_set<UsingColumnSet *>> using_columns_;
  //! Using column sets
  vector<unique_ptr<UsingColumnSet>> using_column_sets_;
};
}  // namespace bustub
