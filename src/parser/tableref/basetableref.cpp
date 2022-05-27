#include "parser/tableref/basetableref.h"

#include "common/field_writer.h"
// #include "parser/keyword_helper.h"

namespace bustub {

string BaseTableRef::ToString() const {
	// string schema = schema_name.empty() ? "" : KeywordHelper::WriteOptionallyQuoted(schema_name) + ".";
	// string result = schema + KeywordHelper::WriteOptionallyQuoted(table_name);
	// return BaseToString(result, column_name_alias);
	return {};
}

bool BaseTableRef::Equals(const TableRef *other_p) const {
	if (!TableRef::Equals(other_p)) {
		return false;
	}
	auto other = (BaseTableRef *)other_p;
	return other->schema_name_ == schema_name_ && other->table_name_ == table_name_ && column_name_alias_ == other->column_name_alias_;
}

void BaseTableRef::Serialize(FieldWriter &writer) const {
	writer.WriteString(schema_name_);
	writer.WriteString(table_name_);
	writer.WriteList<string>(column_name_alias_);
}

unique_ptr<TableRef> BaseTableRef::Deserialize(FieldReader &reader) {
	auto result = std::make_unique<BaseTableRef>();

	result->schema_name_ = reader.ReadRequired<string>();
	result->table_name_ = reader.ReadRequired<string>();
	result->column_name_alias_ = reader.ReadRequiredList<string>();

	return result;
}

unique_ptr<TableRef> BaseTableRef::Copy() {
	auto copy = std::make_unique<BaseTableRef>();

	copy->schema_name_ = schema_name_;
	copy->table_name_ = table_name_;
	copy->column_name_alias_ = column_name_alias_;
	CopyProperties(*copy);

	return copy;
}
} // namespace bustub
