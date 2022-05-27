// //===----------------------------------------------------------------------===//
// //                         DuckDB
// //
// // bustub/parser/tableref/table_function_ref.hpp
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include <string>
// #include <vector>

// #include "parser/parsed_expression.hpp"
// #include "parser/statement/select_statement.hpp"
// #include "parser/tableref.hpp"

// namespace bustub {
// //! Represents a Table producing function
// class TableFunctionRef : public TableRef {
//  public:
//   DUCKDB_API TableFunctionRef();

//   unique_ptr<ParsedExpression> function;
//   vector<string> column_name_alias;

//   // if the function takes a subquery as argument its in here
//   unique_ptr<SelectStatement> subquery;

//  public:
//   string ToString() const override;

//   bool Equals(const TableRef *other_p) const override;

//   unique_ptr<TableRef> Copy() override;

//   //! Serializes a blob into a BaseTableRef
//   void Serialize(FieldWriter &serializer) const override;
//   //! Deserializes a blob back into a BaseTableRef
//   static unique_ptr<TableRef> Deserialize(FieldReader &source);
// };
// }  // namespace bustub