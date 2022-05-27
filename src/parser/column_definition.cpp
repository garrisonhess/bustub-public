#include "parser/column_definition.h"
#include "common/field_writer.h"

namespace bustub {

ColumnDefinition::ColumnDefinition(string name_p, TypeId type_p) : name_(move(name_p)), type(move(type_p)) {
}

ColumnDefinition::ColumnDefinition(string name_p, TypeId type_p, unique_ptr<ParsedExpression> default_value)
    : name_(move(name_p)), type_(type_p), default_value_(move(default_value)) {
}

ColumnDefinition ColumnDefinition::Copy() const {
	ColumnDefinition copy(name_, type_);
	copy.oid_ = oid_;
	copy.default_value_ = default_value_ ? default_value_->Copy() : nullptr;
	// copy.compression_type = compression_type;
	return copy;
}

void ColumnDefinition::Serialize(Serializer &serializer) const {
	FieldWriter writer(serializer);
	writer.WriteString(name_);
	writer.WriteSerializable(type_);
	writer.WriteOptional(default_value_);
	writer.Finalize();
}

ColumnDefinition ColumnDefinition::Deserialize(Deserializer &source) {
	FieldReader reader(source);
	auto column_name = reader.ReadRequired<string>();
	auto column_type = reader.ReadRequiredSerializable<TypeId, TypeId>();
	auto default_value = reader.ReadOptional<ParsedExpression>(nullptr);
	reader.Finalize();

	return {column_name, column_type, move(default_value)};
}

} // namespace bustub
