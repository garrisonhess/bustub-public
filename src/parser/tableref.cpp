#include "parser/tableref.h"
#include <cassert>

#include "common/field_writer.h"
#include "common/printer.h"
#include "parser/keyword_helper.h"

namespace bustub {

string TableRef::BaseToString(string result) const {
  vector<string> column_name_alias;
  return BaseToString(move(result), column_name_alias);
}

string TableRef::BaseToString(string result, const vector<string> &column_name_alias) const {
  if (!alias_.empty()) {
    result += " AS " + KeywordHelper::WriteOptionallyQuoted(alias_);
  }
  if (!column_name_alias.empty()) {
    assert(!alias_.empty());
    result += "(";
    for (uint64_t i = 0; i < column_name_alias.size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += KeywordHelper::WriteOptionallyQuoted(column_name_alias[i]);
    }
    result += ")";
  }

  return result;
}

bool TableRef::Equals(const TableRef *other) const {
  return (other != nullptr) && type_ == other->type_ && alias_ == other->alias_;
}

void TableRef::Serialize(Serializer &serializer) const {
  FieldWriter writer(serializer);
  writer.WriteField<TableReferenceType>(type_);
  writer.WriteString(alias_);
  Serialize(writer);
  writer.Finalize();
}

unique_ptr<TableRef> TableRef::Deserialize(Deserializer &source) {
  FieldReader reader(source);

  auto type = reader.ReadRequired<TableReferenceType>();
  auto alias = reader.ReadRequired<string>();
  // auto sample = reader.ReadOptional<SampleOptions>(nullptr);
  unique_ptr<TableRef> result;
  switch (type) {
    case TableReferenceType::BASE_TABLE:
      // result = BaseTableRef::Deserialize(reader);
      break;
    case TableReferenceType::CROSS_PRODUCT:
      // result = CrossProductRef::Deserialize(reader);
      break;
    case TableReferenceType::JOIN:
      // result = JoinRef::Deserialize(reader);
      break;
    case TableReferenceType::SUBQUERY:
      // result = SubqueryRef::Deserialize(reader);
      break;
    case TableReferenceType::TABLE_FUNCTION:
      // result = TableFunctionRef::Deserialize(reader);
      break;
    case TableReferenceType::EMPTY:
      // result = EmptyTableRef::Deserialize(reader);
      break;
    case TableReferenceType::EXPRESSION_LIST:
      // result = ExpressionListRef::Deserialize(reader);
      break;
    case TableReferenceType::CTE:
    case TableReferenceType::INVALID:
      throw Exception("Unsupported type for TableRef::Deserialize");
  }
  reader.Finalize();

  result->alias_ = alias;
  // result->sample = move(sample);
  return result;
}

void TableRef::CopyProperties(TableRef &target) const {
  // D_ASSERT(type == target.type);
  target.alias_ = alias_;
  target.query_location_ = query_location_;
  // target.sample = sample ? sample->Copy() : nullptr;
}

void TableRef::Print() { Printer::Print(ToString()); }

}  // namespace bustub
