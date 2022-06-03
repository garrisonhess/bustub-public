#include "parser/expression/comparison_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"
#include "parser/expression/cast_expression.h"

namespace bustub {

ComparisonExpression::ComparisonExpression(ExpressionType type, unique_ptr<ParsedExpression> left,
                                           unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::COMPARISON) {
  this->left_ = move(left);
  this->right_ = move(right);
}

string ComparisonExpression::ToString() const { return ToString<ComparisonExpression, ParsedExpression>(*this); }

bool ComparisonExpression::Equals(const ComparisonExpression *a, const ComparisonExpression *b) {
  if (!a->left_->Equals(b->left_.get())) {
    return false;
  }
  if (!a->right_->Equals(b->right_.get())) {
    return false;
  }
  return true;
}

unique_ptr<ParsedExpression> ComparisonExpression::Copy() const {
  auto copy = make_unique<ComparisonExpression>(type_, left_->Copy(), right_->Copy());
  copy->CopyProperties(*this);
  return move(copy);
}

void ComparisonExpression::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*left_);
  writer.WriteSerializable(*right_);
}

unique_ptr<ParsedExpression> ComparisonExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto left_child = reader.ReadRequiredSerializable<ParsedExpression>();
  auto right_child = reader.ReadRequiredSerializable<ParsedExpression>();
  return make_unique<ComparisonExpression>(type, move(left_child), move(right_child));
}

}  // namespace bustub
