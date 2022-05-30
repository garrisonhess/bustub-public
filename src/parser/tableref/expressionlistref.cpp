#include "parser/tableref/expressionlistref.h"

#include "common/field_writer.h"

namespace bustub {

string ExpressionListRef::ToString() const {
  assert(!values_.empty());
  string result = "(VALUES ";
  for (uint64_t row_idx = 0; row_idx < values_.size(); row_idx++) {
    if (row_idx > 0) {
      result += ", ";
    }
    auto &row = values_[row_idx];
    result += "(";
    for (uint64_t col_idx = 0; col_idx < row.size(); col_idx++) {
      if (col_idx > 0) {
        result += ", ";
      }
      result += row[col_idx]->ToString();
    }
    result += ")";
  }
  result += ")";
  return BaseToString(result, expected_names_);
}

bool ExpressionListRef::Equals(const TableRef *other_p) const {
  if (!TableRef::Equals(other_p)) {
    return false;
  }
  auto other = (ExpressionListRef *)other_p;
  if (values_.size() != other->values_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < values_.size(); i++) {
    if (values_[i].size() != other->values_[i].size()) {
      return false;
    }
    for (uint64_t j = 0; j < values_[i].size(); j++) {
      if (!values_[i][j]->Equals(other->values_[i][j].get())) {
        return false;
      }
    }
  }
  return true;
}

unique_ptr<TableRef> ExpressionListRef::Copy() {
  // value list
  auto result = std::make_unique<ExpressionListRef>();
  for (auto &val_list : values_) {
    vector<unique_ptr<ParsedExpression>> new_val_list;
    new_val_list.reserve(val_list.size());
    for (auto &val : val_list) {
      new_val_list.push_back(val->Copy());
    }
    result->values_.push_back(move(new_val_list));
  }
  result->expected_names_ = expected_names_;
  result->expected_types_ = expected_types_;
  CopyProperties(*result);
  return result;
}

void ExpressionListRef::Serialize(FieldWriter &writer) const {
  // writer.WriteList<string>(expected_names_);
  // writer.WriteRegularSerializableList<Type>(expected_types_);
  // auto &serializer = writer.GetSerializer();
  // writer.WriteField<uint32_t>(values_.size());
  // for (uint64_t i = 0; i < values_.size(); i++) {
  // 	serializer.WriteList(values_[i]);
  // }
}

unique_ptr<TableRef> ExpressionListRef::Deserialize(FieldReader &reader) {
  auto result = std::make_unique<ExpressionListRef>();
  // // value list
  // result->expected_names_ = reader.ReadRequiredList<string>();
  // result->expected_types_ = reader.ReadRequiredSerializableList<Type, Type>();
  // uint64_t value_list_size = reader.ReadRequired<uint32_t>();
  // auto &source = reader.GetSource();
  // for (uint64_t i = 0; i < value_list_size; i++) {
  // 	vector<unique_ptr<ParsedExpression>> value_list;
  // 	source.ReadList<ParsedExpression>(value_list);
  // 	result->values_.push_back(move(value_list));
  // }
  return result;
}

}  // namespace bustub
