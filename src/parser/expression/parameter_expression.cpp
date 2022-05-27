#include "parser/expression/parameter_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"
#include "common/types/hash.h"
#include "common/to_string.h"

namespace bustub {

ParameterExpression::ParameterExpression()
    : ParsedExpression(ExpressionType::VALUE_PARAMETER, ExpressionClass::PARAMETER), parameter_nr(0) {
}

string ParameterExpression::ToString() const {
	return "$" + to_string(parameter_nr);
}

unique_ptr<ParsedExpression> ParameterExpression::Copy() const {
	auto copy = make_unique<ParameterExpression>();
	copy->parameter_nr = parameter_nr;
	copy->CopyProperties(*this);
	return move(copy);
}

hash_t ParameterExpression::Hash() const {
	hash_t result = ParsedExpression::Hash();
	return CombineHash(bustub::Hash(parameter_nr), result);
}

void ParameterExpression::Serialize(FieldWriter &writer) const {
	writer.WriteField<idx_t>(parameter_nr);
}

unique_ptr<ParsedExpression> ParameterExpression::Deserialize(ExpressionType type, FieldReader &reader) {
	auto expression = make_unique<ParameterExpression>();
	expression->parameter_nr = reader.ReadRequired<idx_t>();
	return move(expression);
}

} // namespace bustub
