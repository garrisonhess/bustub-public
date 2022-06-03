#include "parser/expression/case_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"

namespace bustub {

CaseExpression::CaseExpression() : ParsedExpression(ExpressionType::CASE_EXPR, ExpressionClass::CASE) {}

string CaseExpression::ToString() const { return ToString<CaseExpression, ParsedExpression>(*this); }

bool CaseExpression::Equals(const CaseExpression *a, const CaseExpression *b) {
  if (a->case_checks_.size() != b->case_checks_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < a->case_checks_.size(); i++) {
    if (!a->case_checks_[i].when_expr_->Equals(b->case_checks_[i].when_expr_.get())) {
      return false;
    }
    if (!a->case_checks_[i].then_expr_->Equals(b->case_checks_[i].then_expr_.get())) {
      return false;
    }
  }
  return a->else_expr_->Equals(b->else_expr_.get());
}

unique_ptr<ParsedExpression> CaseExpression::Copy() const {
  auto copy = make_unique<CaseExpression>();
  copy->CopyProperties(*this);
  for (auto &check : case_checks_) {
    CaseCheck new_check;
    new_check.when_expr_ = check.when_expr_->Copy();
    new_check.then_expr_ = check.then_expr_->Copy();
    copy->case_checks_.push_back(move(new_check));
  }
  copy->else_expr_ = else_expr_->Copy();
  return move(copy);
}

void CaseExpression::Serialize(FieldWriter &writer) const {
  auto &serializer = writer.GetSerializer();
  // we write a list of multiple expressions here
  // in order to write this as a single field we directly use the field writers' internal serializer
  writer.WriteField<uint32_t>(case_checks_.size());
  for (auto &check : case_checks_) {
    check.when_expr_->Serialize(serializer);
    check.then_expr_->Serialize(serializer);
  }
  writer.WriteSerializable<ParsedExpression>(*else_expr_);
}

unique_ptr<ParsedExpression> CaseExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto result = make_unique<CaseExpression>();
  auto &source = reader.GetSource();
  auto count = reader.ReadRequired<uint32_t>();
  for (uint64_t i = 0; i < count; i++) {
    CaseCheck new_check;
    new_check.when_expr_ = ParsedExpression::Deserialize(source);
    new_check.then_expr_ = ParsedExpression::Deserialize(source);
    result->case_checks_.push_back(move(new_check));
  }
  result->else_expr_ = reader.ReadRequiredSerializable<ParsedExpression>();
  return move(result);
}

}  // namespace bustub
