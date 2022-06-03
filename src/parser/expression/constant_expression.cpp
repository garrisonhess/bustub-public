#include "parser/expression/constant_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"
#include "common/hash_util.h"
#include "type/type.h"

namespace bustub {

ConstantExpression::ConstantExpression(const Value &val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value_(val) {}

string ConstantExpression::ToString() const { return value_.ToString(); }

bool ConstantExpression::Equals(const ConstantExpression *a, const ConstantExpression *b) {
  return a->value_.CompareEquals(b->value_) == CmpBool::CmpTrue;
}

hash_t ConstantExpression::Hash() const { return ParsedExpression::Hash(); }

unique_ptr<ParsedExpression> ConstantExpression::Copy() const {
  auto copy = std::make_unique<ConstantExpression>(value_);
  copy->CopyProperties(*this);
  return copy;
}

void ConstantExpression::Serialize(FieldWriter &writer) const { writer.WriteSerializable(value_); }

unique_ptr<ParsedExpression> ConstantExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  // Value value = reader.ReadRequiredSerializable<Value, Value>();
  // return std::make_unique<ConstantExpression>(std::move(value));
  throw NotImplementedException("constant expr deserialize");
}

}  // namespace bustub
