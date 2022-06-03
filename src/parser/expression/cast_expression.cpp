#include "parser/expression/cast_expression.h"

#include "common/exception.h"
#include "common/field_writer.h"
#include "common/helper.h"

namespace bustub {

CastExpression::CastExpression(Type target, unique_ptr<ParsedExpression> child, bool try_cast_p)
    : ParsedExpression(ExpressionType::OPERATOR_CAST, ExpressionClass::CAST),
      cast_type_(target),
      try_cast_(try_cast_p) {
  assert(child);
  this->child_ = move(child);
}

string CastExpression::ToString() const {
  throw NotImplementedException("cast expr tostring");
  // return ToString<CastExpression, ParsedExpression>(*this);
}

bool CastExpression::Equals(const CastExpression *a, const CastExpression *b) {
  if (!a->child_->Equals(b->child_.get())) {
    return false;
  }
  throw NotImplementedException("cast expr equals");
  // if (a->cast_type_ != b->cast_type_) {
  //   return false;
  // }
  // if (a->try_cast_ != b->try_cast_) {
  //   return false;
  // }
  // return true;
}

unique_ptr<ParsedExpression> CastExpression::Copy() const {
  auto copy = make_unique<CastExpression>(cast_type_, child_->Copy(), try_cast_);
  copy->CopyProperties(*this);
  return move(copy);
}

void CastExpression::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*child_);
  writer.WriteSerializable(cast_type_);
  writer.WriteField<bool>(try_cast_);
}

unique_ptr<ParsedExpression> CastExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  // auto child = reader.ReadRequiredSerializable<ParsedExpression>();
  // auto cast_type = reader.ReadRequiredSerializable<Type, Type>();
  // auto try_cast = reader.ReadRequired<bool>();
  // return MakeUniqueBase<ParsedExpression, CastExpression>(cast_type, move(child), try_cast);
  throw NotImplementedException("cast expr deserialize");
}

}  // namespace bustub
