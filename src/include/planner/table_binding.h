//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/table_binding.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/case_insensitive_map.h"
#include "common/common.h"
#include "parser/column_definition.h"
#include "parser/parsed_expression.h"
#include "planner/expression_binder.h"

namespace bustub {
class BindContext;
class BoundQueryNode;
class ColumnRefExpression;
class SubqueryRef;
class LogicalGet;
class TableCatalogEntry;
class TableFunctionCatalogEntry;
class BoundTableFunction;

//! A Binding represents a binding to a table, table-producing function or subquery with a specified table index.
struct Binding {
  Binding(const string &alias, vector<Type> types, vector<string> names, uint64_t index);
  virtual ~Binding() = default;

  //! The alias of the binding
  string alias;
  //! The table index of the binding
  uint64_t index;
  vector<Type> types;
  //! Column names of the subquery
  vector<string> names;
  //! Name -> index for the names
  case_insensitive_map_t<uint64_t> name_map;

 public:
  // bool TryGetBindingIndex(const string &column_name, uint64_t &column_index);
  // uint64_t GetBindingIndex(const string &column_name);
  // bool HasMatchingBinding(const string &column_name);
  virtual string ColumnNotFoundError(const string &column_name) const;
  virtual BindResult Bind(ColumnRefExpression &colref, uint64_t depth);
  virtual TableCatalogEntry *GetTableEntry();
};

//! TableBinding is exactly like the Binding, except it keeps track of which columns were bound in the linked LogicalGet
//! node for projection pushdown purposes.
struct TableBinding : public Binding {
  TableBinding(const string &alias, vector<Type> types, vector<string> names, LogicalGet &get, uint64_t index,
               bool add_row_id = false);

  //! the underlying LogicalGet
  LogicalGet &get;

 public:
  BindResult Bind(ColumnRefExpression &colref, uint64_t depth) override;
  TableCatalogEntry *GetTableEntry() override;
  string ColumnNotFoundError(const string &column_name) const override;
};

}  // namespace bustub
