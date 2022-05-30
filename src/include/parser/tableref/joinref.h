//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/tableref/joinref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/enums/join_type.h"
#include "parser/parsed_expression.h"
#include "parser/tableref.h"

namespace bustub {
//! Represents a JOIN between two expressions
class JoinRef : public TableRef {
 public:
  JoinRef() : TableRef(TableReferenceType::JOIN), is_natural_(false) {}

  //! The left hand side of the join
  unique_ptr<TableRef> left_;
  //! The right hand side of the join
  unique_ptr<TableRef> right_;
  //! The join condition
  unique_ptr<ParsedExpression> condition_;
  //! The join type
  JoinType type_;
  //! Natural join
  bool is_natural_;
  //! The set of USING columns (if any)
  vector<string> using_columns_;

 public:
  string ToString() const override;
  bool Equals(const TableRef *other_p) const override;

  unique_ptr<TableRef> Copy() override;

  //! Serializes a blob into a JoinRef
  void Serialize(FieldWriter &writer) const override;
  //! Deserializes a blob back into a JoinRef
  static unique_ptr<TableRef> Deserialize(FieldReader &reader);
};
}  // namespace bustub
