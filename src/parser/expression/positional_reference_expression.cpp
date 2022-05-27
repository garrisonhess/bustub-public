// #include "parser/expression/positional_reference_expression.h"

// #include "common/exception.h"
// #include "common/field_writer.h"
// #include "common/types/hash.h"
// #include "common/to_string.h"

// namespace bustub {

// PositionalReferenceExpression::PositionalReferenceExpression(uint64_t index)
//     : ParsedExpression(ExpressionType::POSITIONAL_REFERENCE, ExpressionClass::POSITIONAL_REFERENCE), index(index) {
// }

// string PositionalReferenceExpression::ToString() const {
// 	return "#" + to_string(index);
// }

// bool PositionalReferenceExpression::Equals(const PositionalReferenceExpression *a,
//                                            const PositionalReferenceExpression *b) {
// 	return a->index == b->index;
// }

// unique_ptr<ParsedExpression> PositionalReferenceExpression::Copy() const {
// 	auto copy = make_unique<PositionalReferenceExpression>(index);
// 	copy->CopyProperties(*this);
// 	return move(copy);
// }

// hash_t PositionalReferenceExpression::Hash() const {
// 	hash_t result = ParsedExpression::Hash();
// 	return CombineHash(bustub::Hash(index), result);
// }

// void PositionalReferenceExpression::Serialize(FieldWriter &writer) const {
// 	writer.WriteField<uint64_t>(index);
// }

// unique_ptr<ParsedExpression> PositionalReferenceExpression::Deserialize(ExpressionType type, FieldReader &reader) {
// 	auto expression = make_unique<PositionalReferenceExpression>(reader.ReadRequired<uint64_t>());
// 	return move(expression);
// }

// } // namespace bustub
