#include "parser/parsed_expression.h"

#include "common/field_writer.h"
#include "common/hash_util.h"
#include "parser/expression/constant_expression.h"
#include "parser/expression/list.h"
#include "parser/parsed_expression_iterator.h"

namespace bustub {

bool ParsedExpression::IsAggregate() const {
  bool is_aggregate = false;
  ParsedExpressionIterator::EnumerateChildren(
      *this, [&](const ParsedExpression &child) { is_aggregate |= child.IsAggregate(); });
  return is_aggregate;
}

bool ParsedExpression::IsWindow() const { return false; }

bool ParsedExpression::IsScalar() const {
  bool is_scalar = true;
  ParsedExpressionIterator::EnumerateChildren(*this, [&](const ParsedExpression &child) {
    if (!child.IsScalar()) {
      is_scalar = false;
    }
  });
  return is_scalar;
}

bool ParsedExpression::HasParameter() const {
  bool has_parameter = false;
  ParsedExpressionIterator::EnumerateChildren(
      *this, [&](const ParsedExpression &child) { has_parameter |= child.HasParameter(); });
  return has_parameter;
}

bool ParsedExpression::HasSubquery() const {
  bool has_subquery = false;
  ParsedExpressionIterator::EnumerateChildren(
      *this, [&](const ParsedExpression &child) { has_subquery |= child.HasSubquery(); });
  return has_subquery;
}

bool ParsedExpression::Equals(const BaseExpression *other) const {
  return false;

  if (!BaseExpression::Equals(other)) {
    return false;
  }
  switch (expression_class_) {
    case ExpressionClass::COLUMN_REF:
      return ColumnRefExpression::Equals((ColumnRefExpression *)this, (ColumnRefExpression *)other);
    case ExpressionClass::CONSTANT:
      return ConstantExpression::Equals((ConstantExpression *)this, (ConstantExpression *)other);
    case ExpressionClass::DEFAULT:
      return true;
    case ExpressionClass::STAR:
      return StarExpression::Equals((StarExpression *)this, (StarExpression *)other);
    case ExpressionClass::SUBQUERY:
    case ExpressionClass::WINDOW:
    case ExpressionClass::POSITIONAL_REFERENCE:
    case ExpressionClass::COLLATE:
    case ExpressionClass::PARAMETER:
    case ExpressionClass::OPERATOR:
    case ExpressionClass::LAMBDA:
    case ExpressionClass::FUNCTION:
    case ExpressionClass::CONJUNCTION:
    case ExpressionClass::BETWEEN:
    case ExpressionClass::CASE:
    case ExpressionClass::CAST:
    case ExpressionClass::COMPARISON:
      throw Exception("Unsupported type for expression comparison!");
    default:
      throw Exception("Unsupported type for expression comparison!");
  }
}

hash_t ParsedExpression::Hash() const {
  hash_t hash = HashUtil::Hash(&type_);
  ParsedExpressionIterator::EnumerateChildren(
      *this, [&](const ParsedExpression &child) { hash = HashUtil::CombineHashes(child.Hash(), hash); });
  return hash;
}

void ParsedExpression::Serialize(Serializer &serializer) const {
  FieldWriter writer(serializer);
  writer.WriteField<ExpressionClass>(GetExpressionClass());
  writer.WriteField<ExpressionType>(type_);
  writer.WriteString(alias_);
  Serialize(writer);
  writer.Finalize();
}

unique_ptr<ParsedExpression> ParsedExpression::Deserialize(Deserializer &source) {
  FieldReader reader(source);
  auto expression_class = reader.ReadRequired<ExpressionClass>();
  auto type = reader.ReadRequired<ExpressionType>();
  auto alias = reader.ReadRequired<string>();
  unique_ptr<ParsedExpression> result;
  switch (expression_class) {
    case ExpressionClass::DEFAULT:
      result = DefaultExpression::Deserialize(type, reader);
      break;

    case ExpressionClass::STAR:
      result = StarExpression::Deserialize(type, reader);
      break;
    case ExpressionClass::SUBQUERY:
    case ExpressionClass::WINDOW:
    case ExpressionClass::FUNCTION:
    case ExpressionClass::LAMBDA:
    case ExpressionClass::OPERATOR:
    case ExpressionClass::PARAMETER:
    case ExpressionClass::POSITIONAL_REFERENCE:
    case ExpressionClass::BETWEEN:
    case ExpressionClass::CASE:
    case ExpressionClass::CAST:
    case ExpressionClass::COLLATE:
    case ExpressionClass::COLUMN_REF:
    case ExpressionClass::COMPARISON:
    case ExpressionClass::CONJUNCTION:
    case ExpressionClass::CONSTANT:
      throw Exception("Unsupported type for expression deserialization!");
    default:
      throw Exception("Unsupported type for expression deserialization!");
  }
  result->alias_ = alias;
  reader.Finalize();
  return result;
}

}  // namespace bustub
