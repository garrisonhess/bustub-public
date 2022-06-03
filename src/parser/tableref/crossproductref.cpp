#include "parser/tableref/crossproductref.h"
#include <cassert>

#include "common/field_writer.h"

namespace bustub {

string CrossProductRef::ToString() const { return left_->ToString() + ", " + right_->ToString(); }

bool CrossProductRef::Equals(const TableRef *other_p) const {
  if (!TableRef::Equals(other_p)) {
    return false;
  }
  auto other = (CrossProductRef *)other_p;
  return left_->Equals(other->left_.get()) && right_->Equals(other->right_.get());
}

unique_ptr<TableRef> CrossProductRef::Copy() {
  auto copy = std::make_unique<CrossProductRef>();
  copy->left_ = left_->Copy();
  copy->right_ = right_->Copy();
  copy->alias_ = alias_;
  return move(copy);
}

void CrossProductRef::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*left_);
  writer.WriteSerializable(*right_);
}

unique_ptr<TableRef> CrossProductRef::Deserialize(FieldReader &reader) {
  auto result = std::make_unique<CrossProductRef>();

  result->left_ = reader.ReadRequiredSerializable<TableRef>();
  result->right_ = reader.ReadRequiredSerializable<TableRef>();
  assert(result->left_);
  assert(result->right_);

  return move(result);
}

}  // namespace bustub
