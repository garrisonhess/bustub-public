#include "parser/expression/conjunction_expression.h"
#include "common/exception.h"
#include "common/field_writer.h"
#include "parser/expression_util.h"

namespace bustub {

ConjunctionExpression::ConjunctionExpression(ExpressionType type)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {}

ConjunctionExpression::ConjunctionExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {
  for (auto &child : children) {
    AddExpression(move(child));
  }
}

ConjunctionExpression::ConjunctionExpression(ExpressionType type, unique_ptr<ParsedExpression> left,
                                             unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION) {
  AddExpression(move(left));
  AddExpression(move(right));
}

void ConjunctionExpression::AddExpression(unique_ptr<ParsedExpression> expr) {
  if (expr->type_ == type_) {
    // expr is a conjunction of the same type: merge the expression lists together
    auto &other = (ConjunctionExpression &)*expr;
    for (auto &child : other.children_) {
      children_.push_back(move(child));
    }
  } else {
    children_.push_back(move(expr));
  }
}

string ConjunctionExpression::ToString() const { return ToString<ConjunctionExpression, ParsedExpression>(*this); }

bool ConjunctionExpression::Equals(const ConjunctionExpression *a, const ConjunctionExpression *b) {
  return ExpressionUtil::SetEquals(a->children_, b->children_);
}

unique_ptr<ParsedExpression> ConjunctionExpression::Copy() const {
  vector<unique_ptr<ParsedExpression>> copy_children;
  for (auto &expr : children_) {
    copy_children.push_back(expr->Copy());
  }
  auto copy = make_unique<ConjunctionExpression>(type_, move(copy_children));
  copy->CopyProperties(*this);
  return move(copy);
}

void ConjunctionExpression::Serialize(FieldWriter &writer) const { writer.WriteSerializableList(children_); }

unique_ptr<ParsedExpression> ConjunctionExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto result = make_unique<ConjunctionExpression>(type);
  result->children_ = reader.ReadRequiredSerializableList<ParsedExpression>();
  return move(result);
}

}  // namespace bustub
