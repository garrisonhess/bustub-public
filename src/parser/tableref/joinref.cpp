#include "parser/tableref/joinref.h"

#include "common/field_writer.h"
#include "common/limits.h"
#include "parser/parsed_expression.h"

namespace bustub {

string JoinRef::ToString() const {
  string result;
  result = left_->ToString() + " ";
  if (is_natural_) {
    result += "NATURAL ";
  }
  result += JoinTypeToString(type_) + " JOIN ";
  result += right_->ToString();
  if (condition_) {
    assert(using_columns_.empty());
    result += " ON (";
    result += condition_->ToString();
    result += ")";
  } else if (!using_columns_.empty()) {
    result += " USING (";
    for (uint64_t i = 0; i < using_columns_.size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += using_columns_[i];
    }
    result += ")";
  }
  return result;
}

bool JoinRef::Equals(const TableRef *other_p) const {
  if (!TableRef::Equals(other_p)) {
    return false;
  }
  auto other = (JoinRef *)other_p;
  if (using_columns_.size() != other->using_columns_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < using_columns_.size(); i++) {
    if (using_columns_[i] != other->using_columns_[i]) {
      return false;
    }
  }
  return left_->Equals(other->left_.get()) && right_->Equals(other->right_.get()) &&
         BaseExpression::Equals(condition_.get(), other->condition_.get()) && type_ == other->type_;
}

unique_ptr<TableRef> JoinRef::Copy() {
  auto copy = std::make_unique<JoinRef>();
  copy->left_ = left_->Copy();
  copy->right_ = right_->Copy();
  if (condition_) {
    copy->condition_ = condition_->Copy();
  }
  copy->type_ = type_;
  copy->is_natural_ = is_natural_;
  copy->alias_ = alias_;
  copy->using_columns_ = using_columns_;
  return copy;
}

void JoinRef::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*left_);
  writer.WriteSerializable(*right_);
  writer.WriteOptional(condition_);
  writer.WriteField<JoinType>(type_);
  writer.WriteField<bool>(is_natural_);
  writer.WriteList<string>(using_columns_);
}

unique_ptr<TableRef> JoinRef::Deserialize(FieldReader &reader) {
  auto result = std::make_unique<JoinRef>();
  result->left_ = reader.ReadRequiredSerializable<TableRef>();
  result->right_ = reader.ReadRequiredSerializable<TableRef>();
  result->condition_ = reader.ReadOptional<ParsedExpression>(nullptr);
  result->type_ = reader.ReadRequired<JoinType>();
  result->is_natural_ = reader.ReadRequired<bool>();
  result->using_columns_ = reader.ReadRequiredList<string>();
  return result;
}

}  // namespace bustub
