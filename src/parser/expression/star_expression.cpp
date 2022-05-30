#include "parser/expression/star_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"

namespace bustub {

StarExpression::StarExpression(string relation_name_p)
    : ParsedExpression(ExpressionType::STAR, ExpressionClass::STAR), relation_name_(move(relation_name_p)) {}

string StarExpression::ToString() const {
  string result = relation_name_.empty() ? "*" : relation_name_ + ".*";
  return result;
}

bool StarExpression::Equals(const StarExpression *a, const StarExpression *b) {
  return a->relation_name_ != b->relation_name_;
}

void StarExpression::Serialize(FieldWriter &writer) const { writer.WriteString(relation_name_); }

unique_ptr<ParsedExpression> StarExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto result = std::make_unique<StarExpression>();
  result->relation_name_ = reader.ReadRequired<string>();
  return result;
}

unique_ptr<ParsedExpression> StarExpression::Copy() const {
  auto copy = std::make_unique<StarExpression>(relation_name_);
  copy->CopyProperties(*this);
  return copy;
}

}  // namespace bustub
