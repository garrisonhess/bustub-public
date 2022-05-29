// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // bustub/parser/tableref/subqueryref.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "parser/statement/select_statement.h"
// #include "parser/tableref.h"

// namespace bustub {
// //! Represents a subquery
// class SubqueryRef : public TableRef {
//  public:
//   explicit SubqueryRef(unique_ptr<SelectStatement> subquery, string alias = string());

//   //! The subquery
//   unique_ptr<SelectStatement> subquery;
//   //! Aliases for the column names
//   vector<string> column_name_alias;

//  public:
//   string ToString() const override;
//   bool Equals(const TableRef *other_p) const override;

//   unique_ptr<TableRef> Copy() override;

//   //! Serializes a blob into a SubqueryRef
//   void Serialize(FieldWriter &serializer) const override;
//   //! Deserializes a blob back into a SubqueryRef
//   static unique_ptr<TableRef> Deserialize(FieldReader &source);
// };
// }  // namespace bustub
