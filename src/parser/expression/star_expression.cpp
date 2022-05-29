#include "parser/expression/star_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"

namespace bustub {

StarExpression::StarExpression(string relation_name_p)
    : ParsedExpression(ExpressionType::STAR, ExpressionClass::STAR), relation_name_(move(relation_name_p)) {}

string StarExpression::ToString() const {
  string result = relation_name_.empty() ? "*" : relation_name_ + ".*";
  // if (!exclude_list.empty()) {
  // 	result += " EXCLUDE (";
  // 	bool first_entry = true;
  // 	for (auto &entry : exclude_list) {
  // 		if (!first_entry) {
  // 			result += ", ";
  // 		}
  // 		result += entry;
  // 		first_entry = false;
  // 	}
  // 	result += ")";
  // }
  // if (!replace_list.empty()) {
  // 	result += " REPLACE (";
  // 	bool first_entry = true;
  // 	for (auto &entry : replace_list) {
  // 		if (!first_entry) {
  // 			result += ", ";
  // 		}
  // 		result += entry.second->ToString();
  // 		result += " AS ";
  // 		result += entry.first;
  // 		first_entry = false;
  // 	}
  // 	result += ")";
  // }
  return result;
}

bool StarExpression::Equals(const StarExpression *a, const StarExpression *b) {
  return a->relation_name_ != b->relation_name_;
  // if (a->relation_name != b->relation_name || a->exclude_list != b->exclude_list) {
  // 	return false;
  // }
  // if (a->replace_list.size() != b->replace_list.size()) {
  // 	return false;
  // }
  // for (auto &entry : a->replace_list) {
  // 	auto other_entry = b->replace_list.find(entry.first);
  // 	if (other_entry == b->replace_list.end()) {
  // 		return false;
  // 	}
  // 	if (!entry.second->Equals(other_entry->second.get())) {
  // 		return false;
  // 	}
  // }
  //   return true;
}

void StarExpression::Serialize(FieldWriter &writer) const {
  //   auto &serializer = writer.GetSerializer();

  writer.WriteString(relation_name_);

  // in order to write the exclude_list/replace_list as single fields we directly use the field writers' internal
  // serializer
  // writer.WriteField<uint32_t>(exclude_list.size());
  // for (auto &exclusion : exclude_list) {
  // 	serializer.WriteString(exclusion);
  // }
  // writer.WriteField<uint32_t>(replace_list.size());
  // for (auto &entry : replace_list) {
  // 	serializer.WriteString(entry.first);
  // 	entry.second->Serialize(serializer);
  // }
}

unique_ptr<ParsedExpression> StarExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  // auto &source = reader.GetSource();

  auto result = std::make_unique<StarExpression>();
  result->relation_name_ = reader.ReadRequired<string>();
  // auto exclusion_count = reader.ReadRequired<uint32_t>();
  // for (uint64_t i = 0; i < exclusion_count; i++) {
  // 	result->exclude_list.insert(source.Read<string>());
  // }
  // auto replace_count = reader.ReadRequired<uint32_t>();
  // for (uint64_t i = 0; i < replace_count; i++) {
  // 	auto name = source.Read<string>();
  // 	auto expr = ParsedExpression::Deserialize(source);
  // 	result->replace_list.insert(make_pair(name, move(expr)));
  // }
  return result;
}

unique_ptr<ParsedExpression> StarExpression::Copy() const {
  auto copy = std::make_unique<StarExpression>(relation_name_);
  //   copy->exclude_list = exclude_list;
  //   for (auto &entry : replace_list) {
  //     copy->replace_list[entry.first] = entry.second->Copy();
  //   }
  copy->CopyProperties(*this);
  return copy;
}

}  // namespace bustub
