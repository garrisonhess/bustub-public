//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression/default_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {
using std::string;

//! Represents the default value of a column
class DefaultExpression : public ParsedExpression {
 public:
  DefaultExpression();

 public:
  bool IsScalar() const override { return false; }

  string ToString() const override;

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &source);
};
}  // namespace bustub
