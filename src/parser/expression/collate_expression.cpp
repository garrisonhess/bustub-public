#include "bustub/parser/expression/collate_expression.hpp"

#include "bustub/common/exception.hpp"
#include "bustub/common/field_writer.hpp"

namespace bustub {

CollateExpression::CollateExpression(string collation_p, unique_ptr<ParsedExpression> child)
    : ParsedExpression(ExpressionType::COLLATE, ExpressionClass::COLLATE), collation(move(collation_p)) {
	D_ASSERT(child);
	this->child = move(child);
}

string CollateExpression::ToString() const {
	return child->ToString() + " COLLATE " + collation;
}

bool CollateExpression::Equals(const CollateExpression *a, const CollateExpression *b) {
	if (!a->child->Equals(b->child.get())) {
		return false;
	}
	if (a->collation != b->collation) {
		return false;
	}
	return true;
}

unique_ptr<ParsedExpression> CollateExpression::Copy() const {
	auto copy = make_unique<CollateExpression>(collation, child->Copy());
	copy->CopyProperties(*this);
	return move(copy);
}

void CollateExpression::Serialize(FieldWriter &writer) const {
	writer.WriteSerializable(*child);
	writer.WriteString(collation);
}

unique_ptr<ParsedExpression> CollateExpression::Deserialize(ExpressionType type, FieldReader &reader) {
	auto child = reader.ReadRequiredSerializable<ParsedExpression>();
	auto collation = reader.ReadRequired<string>();
	return make_unique_base<ParsedExpression, CollateExpression>(collation, move(child));
}

} // namespace bustub
