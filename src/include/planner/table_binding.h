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
  bool TryGetBindingIndex(const string &column_name, uint64_t &column_index);
  uint64_t GetBindingIndex(const string &column_name);
  bool HasMatchingBinding(const string &column_name);
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

//! MacroBinding is like the Binding, except the alias and index are set by default. Used for binding Macro
//! Params/Arguments.
struct MacroBinding : public Binding {
  static constexpr const char *MACRO_NAME = "0_macro_parameters";

 public:
  MacroBinding(vector<Type> types_p, vector<string> names_p, string macro_name);

  //! Arguments
  vector<unique_ptr<ParsedExpression>> arguments;
  //! The name of the macro
  string macro_name;

 public:
  BindResult Bind(ColumnRefExpression &colref, uint64_t depth) override;

  //! Given the parameter colref, returns a copy of the argument that was supplied for this parameter
  unique_ptr<ParsedExpression> ParamToArg(ColumnRefExpression &colref);
};

}  // namespace bustub
