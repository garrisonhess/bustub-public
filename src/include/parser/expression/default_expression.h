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
//! Represents the default value of a column
class DefaultExpression : public ParsedExpression {
 public:
  DefaultExpression();

 public:
  bool IsScalar() const override { return false; }

  std::string ToString() const override;

  std::unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &source);
};
}  // namespace bustub
