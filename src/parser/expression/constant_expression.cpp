#include "bustub/parser/expression/constant_expression.hpp"

#include "bustub/common/exception.hpp"
#include "bustub/common/field_writer.hpp"
#include "bustub/common/types/hash.hpp"
#include "bustub/common/value_operations/value_operations.hpp"

namespace bustub {

ConstantExpression::ConstantExpression(Value val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value(move(val)) {
}

string ConstantExpression::ToString() const {
	return value.ToSQLString();
}

bool ConstantExpression::Equals(const ConstantExpression *a, const ConstantExpression *b) {
	return !ValueOperations::DistinctFrom(a->value, b->value);
}

hash_t ConstantExpression::Hash() const {
	return ParsedExpression::Hash();
}

unique_ptr<ParsedExpression> ConstantExpression::Copy() const {
	auto copy = make_unique<ConstantExpression>(value);
	copy->CopyProperties(*this);
	return move(copy);
}

void ConstantExpression::Serialize(FieldWriter &writer) const {
	writer.WriteSerializable(value);
}

unique_ptr<ParsedExpression> ConstantExpression::Deserialize(ExpressionType type, FieldReader &reader) {
	Value value = reader.ReadRequiredSerializable<Value, Value>();
	return make_unique<ConstantExpression>(move(value));
}

} // namespace bustub
