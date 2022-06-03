//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/between_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"

namespace bustub {

class BetweenExpression : public ParsedExpression {
 public:
  BetweenExpression(unique_ptr<ParsedExpression> input, unique_ptr<ParsedExpression> lower,
                    unique_ptr<ParsedExpression> upper);

  unique_ptr<ParsedExpression> input_;
  unique_ptr<ParsedExpression> lower_;
  unique_ptr<ParsedExpression> upper_;

 public:
  string ToString() const override;

  static bool Equals(const BetweenExpression *a, const BetweenExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE>
  static string ToString(const T &entry) {
    return entry.input_->ToString() + " BETWEEN " + entry.lower_->ToString() + " AND " + entry.upper_->ToString();
  }
};
}  // namespace bustub
