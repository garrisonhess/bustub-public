//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression/constant_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include "parser/parsed_expression.h"
#include "type/value.h"

namespace bustub {
using std::string;

//! ConstantExpression represents a constant value in the query
class ConstantExpression : public ParsedExpression {
 public:
  explicit ConstantExpression(Value val);

  //! The constant value referenced
  Value value_;

 public:
  string ToString() const override;

  static bool Equals(const ConstantExpression *a, const ConstantExpression *b);
  hash_t Hash() const override;

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);
};

}  // namespace bustub
