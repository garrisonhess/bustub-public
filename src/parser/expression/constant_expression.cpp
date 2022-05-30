#include "parser/expression/constant_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"
#include "common/hash_util.h"
// #include "common/value_operations/value_operations.h"

namespace bustub {

ConstantExpression::ConstantExpression(Value val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value_(std::move(val)) {}

string ConstantExpression::ToString() const {
  throw NotImplementedException("const expr not impl");
  // return value_.ToSQLString();
}

bool ConstantExpression::Equals(const ConstantExpression *a, const ConstantExpression *b) {
  return false;
  // return !ValueOperations::DistinctFrom(a->value_, b->value_);
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
  return nullptr;
}

}  // namespace bustub
