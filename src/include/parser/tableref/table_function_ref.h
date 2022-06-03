//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/tableref/table_function_ref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "parser/parsed_expression.h"
#include "parser/statement/select_statement.h"
#include "parser/tableref.h"

namespace bustub {
//! Represents a Table producing function
class TableFunctionRef : public TableRef {
 public:
  explicit TableFunctionRef();

  unique_ptr<ParsedExpression> function_;
  vector<string> column_name_alias_;

  // if the function takes a subquery as argument its in here
  unique_ptr<SelectStatement> subquery_;

 public:
  string ToString() const override;

  bool Equals(const TableRef *other_p) const override;

  unique_ptr<TableRef> Copy() override;

  //! Serializes a blob into a BaseTableRef
  void Serialize(FieldWriter &writer) const override;
  //! Deserializes a blob back into a BaseTableRef
  static unique_ptr<TableRef> Deserialize(FieldReader &reader);
};
}  // namespace bustub
