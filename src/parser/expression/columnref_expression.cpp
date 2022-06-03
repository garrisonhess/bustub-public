#include "parser/expression/columnref_expression.h"

#include "common/field_writer.h"
#include "common/string_util.h"
#include "parser/keyword_helper.h"

namespace bustub {

ColumnRefExpression::ColumnRefExpression(string column_name, string table_name)
    : ColumnRefExpression(table_name.empty() ? vector<string>{move(column_name)}
                                             : vector<string>{move(table_name), move(column_name)}) {}

ColumnRefExpression::ColumnRefExpression(string column_name) : ColumnRefExpression(vector<string>{move(column_name)}) {}

ColumnRefExpression::ColumnRefExpression(vector<string> column_names_p)
    : ParsedExpression(ExpressionType::COLUMN_REF, ExpressionClass::COLUMN_REF), column_names_(move(column_names_p)) {
  // #ifdef DEBUG
  for (auto &col_name : column_names_) {
    assert(!col_name.empty());
  }
  // #endif
}

bool ColumnRefExpression::IsQualified() const { return column_names_.size() > 1; }

const string &ColumnRefExpression::GetColumnName() const {
  assert(column_names_.size() <= 3);
  return column_names_.back();
}

const string &ColumnRefExpression::GetTableName() const {
  assert(column_names_.size() >= 2 && column_names_.size() <= 3);
  return column_names_.size() == 3 ? column_names_[1] : column_names_[0];
}

string ColumnRefExpression::GetName() const { return !alias_.empty() ? alias_ : column_names_.back(); }

string ColumnRefExpression::ToString() const {
  string result;
  for (uint64_t i = 0; i < column_names_.size(); i++) {
    if (i > 0) {
      result += ".";
    }
    result += KeywordHelper::WriteOptionallyQuoted(column_names_[i]);
  }
  return result;
}

bool ColumnRefExpression::Equals(const ColumnRefExpression *a, const ColumnRefExpression *b) {
  if (a->column_names_.size() != b->column_names_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < a->column_names_.size(); i++) {
    auto lcase_a = StringUtil::Lower(a->column_names_[i]);
    auto lcase_b = StringUtil::Lower(b->column_names_[i]);
    if (lcase_a != lcase_b) {
      return false;
    }
  }
  return true;
}

hash_t ColumnRefExpression::Hash() const {
  hash_t result = ParsedExpression::Hash();
  for (auto &column_name : column_names_) {
    auto lcase = StringUtil::Lower(column_name);
    // TODO(GH): verify this is correct.
    result = HashUtil::CombineHashes(result, HashUtil::Hash(lcase.c_str()));
  }
  return result;
}

unique_ptr<ParsedExpression> ColumnRefExpression::Copy() const {
  auto copy = std::make_unique<ColumnRefExpression>(column_names_);
  copy->CopyProperties(*this);
  return copy;
}

void ColumnRefExpression::Serialize(FieldWriter &writer) const { writer.WriteList<string>(column_names_); }

unique_ptr<ParsedExpression> ColumnRefExpression::Deserialize(ExpressionType type, FieldReader &reader) {
  auto column_names = reader.ReadRequiredList<string>();
  auto expression = std::make_unique<ColumnRefExpression>(move(column_names));
  return expression;
}

}  // namespace bustub
