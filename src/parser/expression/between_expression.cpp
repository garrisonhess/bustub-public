#include "parser/expression/between_expression.h"
#include "common/field_writer.h"

namespace bustub {

BetweenExpression::BetweenExpression(unique_ptr<ParsedExpression> input_p, unique_ptr<ParsedExpression> lower_p,
                                     unique_ptr<ParsedExpression> upper_p)
    : ParsedExpression(ExpressionType::COMPARE_BETWEEN, ExpressionClass::BETWEEN),
      input_(move(input_p)),
      lower_(move(lower_p)),
      upper_(move(upper_p)) {}

string BetweenExpression::ToString() const { return ToString<BetweenExpression, ParsedExpression>(*this); }

bool BetweenExpression::Equals(const BetweenExpression *a, const BetweenExpression *b) {
  if (!a->input_->Equals(b->input_.get())) {
    return false;
  }
  if (!a->lower_->Equals(b->lower_.get())) {
    return false;
  }
  if (!a->upper_->Equals(b->upper_.get())) {
    return false;
  }
  return true;
}

unique_ptr<ParsedExpression> BetweenExpression::Copy() const {
  auto copy = make_unique<BetweenExpression>(input_->Copy(), lower_->Copy(), upper_->Copy());
  copy->CopyProperties(*this);
  return move(copy);
}

void BetweenExpression::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*input_);
  writer.WriteSerializable(*lower_);
  writer.WriteSerializable(*upper_);
}

unique_ptr<ParsedExpression> BetweenExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto input = reader.ReadRequiredSerializable<ParsedExpression>();
  auto lower = reader.ReadRequiredSerializable<ParsedExpression>();
  auto upper = reader.ReadRequiredSerializable<ParsedExpression>();
  return make_unique<BetweenExpression>(move(input), move(lower), move(upper));
}

}  // namespace bustub