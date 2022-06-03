//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/case_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "parser/parsed_expression.h"

namespace bustub {

struct CaseCheck {
  unique_ptr<ParsedExpression> when_expr_;
  unique_ptr<ParsedExpression> then_expr_;
};

//! The CaseExpression represents a CASE expression in the query
class CaseExpression : public ParsedExpression {
 public:
  CaseExpression();

  vector<CaseCheck> case_checks_;
  unique_ptr<ParsedExpression> else_expr_;

 public:
  string ToString() const override;

  static bool Equals(const CaseExpression *a, const CaseExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE>
  static string ToString(const T &entry) {
    string case_str = "CASE ";
    for (auto &check : entry.case_checks_) {
      case_str += " WHEN (" + check.when_expr_->ToString() + ")";
      case_str += " THEN (" + check.then_expr_->ToString() + ")";
    }
    case_str += " ELSE " + entry.else_expr_->ToString();
    case_str += " END";
    return case_str;
  }
};
}  // namespace bustub
