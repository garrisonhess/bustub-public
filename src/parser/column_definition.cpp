#include "parser/column_definition.h"
#include "common/field_writer.h"
#include "parser/parsed_expression.h"

namespace bustub {

ColumnDefinition::ColumnDefinition(string name_p, const Type &type_p) : name_(move(name_p)), type_(type_p) {}

ColumnDefinition::ColumnDefinition(string name_p, const Type &type_p, unique_ptr<ParsedExpression> default_value)
    : name_(move(name_p)), type_(type_p), default_value_(std::move(default_value)) {}

ColumnDefinition ColumnDefinition::Copy() const {
  ColumnDefinition copy(name_, type_);
  copy.oid_ = oid_;
  copy.default_value_ = default_value_ ? default_value_->Copy() : nullptr;
  return copy;
}

void ColumnDefinition::Serialize(Serializer &serializer) const {
  FieldWriter writer(serializer);
  writer.WriteString(name_);
  writer.WriteSerializable(type_);
  writer.WriteOptional(default_value_);
  writer.Finalize();
}

// ColumnDefinition ColumnDefinition::Deserialize(Deserializer &source) {
// 	// FieldReader reader(source);
// 	// auto column_name = reader.ReadRequired<string>();
// 	// auto column_type = reader.ReadRequiredSerializable<Type, Type>();
// 	// auto default_value = reader.ReadOptional<ParsedExpression>(nullptr);
// 	// reader.Finalize();
// 	// return {column_name, column_type, std::move(default_value)};
// }

}  // namespace bustub
