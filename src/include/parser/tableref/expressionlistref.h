//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/tableref/expressionlistref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "parser/parsed_expression.h"
#include "parser/tableref.h"
#include "type/type_id.h"

namespace bustub {
//! Represents an expression list as generated by a VALUES statement
class ExpressionListRef : public TableRef {
 public:
  ExpressionListRef() : TableRef(TableReferenceType::EXPRESSION_LIST) {}

  //! Value list, only used for VALUES statement
  vector<vector<unique_ptr<ParsedExpression>>> values_;
  //! Expected SQL types
  vector<TypeId> expected_types_;
  //! The set of expected names
  vector<string> expected_names_;

 public:
  string ToString() const override;
  bool Equals(const TableRef *other_p) const override;

  unique_ptr<TableRef> Copy() override;

  //! Serializes a blob into a ExpressionListRef
  void Serialize(FieldWriter &serializer) const override;
  //! Deserializes a blob back into a ExpressionListRef
  static unique_ptr<TableRef> Deserialize(FieldReader &source);
};
}  // namespace bustub
