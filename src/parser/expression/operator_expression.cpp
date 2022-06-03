#include "parser/expression/operator_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"

namespace bustub {

OperatorExpression::OperatorExpression(ExpressionType type, unique_ptr<ParsedExpression> left,
                                       unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::OPERATOR) {
  if (left) {
    children_.push_back(move(left));
  }
  if (right) {
    children_.push_back(move(right));
  }
}

OperatorExpression::OperatorExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children)
    : ParsedExpression(type, ExpressionClass::OPERATOR), children_(move(children)) {}

string OperatorExpression::ToString() const { return ToString<OperatorExpression, ParsedExpression>(*this); }

bool OperatorExpression::Equals(const OperatorExpression *a, const OperatorExpression *b) {
  if (a->children_.size() != b->children_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < a->children_.size(); i++) {
    if (!a->children_[i]->Equals(b->children_[i].get())) {
      return false;
    }
  }
  return true;
}

unique_ptr<ParsedExpression> OperatorExpression::Copy() const {
  auto copy = make_unique<OperatorExpression>(type_);
  copy->CopyProperties(*this);
  for (auto &it : children_) {
    copy->children_.push_back(it->Copy());
  }
  return move(copy);
}

void OperatorExpression::Serialize(FieldWriter &writer) const { writer.WriteSerializableList(children_); }

unique_ptr<ParsedExpression> OperatorExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto expression = make_unique<OperatorExpression>(type);
  expression->children_ = reader.ReadRequiredSerializableList<ParsedExpression>();
  return move(expression);
}

}  // namespace bustub
