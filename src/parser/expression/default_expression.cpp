#include "parser/expression/default_expression.h"

#include <string>
#include "common/exception.h"

namespace bustub {
using std::make_unique;
using std::string;

DefaultExpression::DefaultExpression() : ParsedExpression(ExpressionType::VALUE_DEFAULT, ExpressionClass::DEFAULT) {}

string DefaultExpression::ToString() const { return "DEFAULT"; }

unique_ptr<ParsedExpression> DefaultExpression::Copy() const {
  auto copy = make_unique<DefaultExpression>();
  copy->CopyProperties(*this);
  return copy;
}

void DefaultExpression::Serialize(FieldWriter &writer) const {}

unique_ptr<ParsedExpression> DefaultExpression::Deserialize(ExpressionType type, FieldReader &source) {
  return make_unique<DefaultExpression>();
}

}  // namespace bustub
