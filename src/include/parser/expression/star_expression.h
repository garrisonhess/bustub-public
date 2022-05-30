//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression/star_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {

using std::string;

//! Represents a * expression in the SELECT clause
class StarExpression : public ParsedExpression {
 public:
  explicit StarExpression(string relation_name = string());

  //! The relation name in case of tbl.*, or empty if this is a normal *
  string relation_name_;

 public:
  string ToString() const override;

  static bool Equals(const StarExpression *a, const StarExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};
}  // namespace bustub
