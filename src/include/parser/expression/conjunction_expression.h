//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/conjunction_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "parser/parsed_expression.h"

namespace bustub {

//! Represents a conjunction (AND/OR)
class ConjunctionExpression : public ParsedExpression {
 public:
  explicit ConjunctionExpression(ExpressionType type);
  ConjunctionExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children);
  ConjunctionExpression(ExpressionType type, unique_ptr<ParsedExpression> left, unique_ptr<ParsedExpression> right);

  vector<unique_ptr<ParsedExpression>> children_;

 public:
  void AddExpression(unique_ptr<ParsedExpression> expr);

  string ToString() const override;

  static bool Equals(const ConjunctionExpression *a, const ConjunctionExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE>
  static string ToString(const T &entry) {
    string result = "(" + entry.children_[0]->ToString();
    for (uint64_t i = 1; i < entry.children_.size(); i++) {
      result += " " + ExpressionTypeToOperator(entry.type_) + " " + entry.children_[i]->ToString();
    }
    return result + ")";
  }
};
}  // namespace bustub
