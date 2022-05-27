//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression/star_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

// #include "common/case_insensitive_map.h"
#include "parser/parsed_expression.h"

namespace bustub {

using std::string;

//! Represents a * expression in the SELECT clause
class StarExpression : public ParsedExpression {
 public:
  explicit StarExpression(string relation_name = string());

  //! The relation name in case of tbl.*, or empty if this is a normal *
  string relation_name_;
  //   //! List of columns to exclude from the STAR expression
  //   case_insensitive_set_t exclude_list;
  //   //! List of columns to replace with another expression
  //   case_insensitive_map_t<unique_ptr<ParsedExpression>> replace_list;

 public:
  string ToString() const override;

  static bool Equals(const StarExpression *a, const StarExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &source);
};
}  // namespace bustub