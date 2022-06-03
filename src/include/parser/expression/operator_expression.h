//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/operator_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "common/string_util.h"
#include "parser/parsed_expression.h"
#include "parser/qualified_name.h"

namespace bustub {
//! Represents a built-in operator expression
class OperatorExpression : public ParsedExpression {
 public:
  explicit OperatorExpression(ExpressionType type, unique_ptr<ParsedExpression> left = nullptr,
                              unique_ptr<ParsedExpression> right = nullptr);
  OperatorExpression(ExpressionType type, vector<unique_ptr<ParsedExpression>> children);

  vector<unique_ptr<ParsedExpression>> children_;

 public:
  string ToString() const override;

  static bool Equals(const OperatorExpression *a, const OperatorExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE>
  static string ToString(const T &entry) {
    auto op = ExpressionTypeToOperator(entry.type_);
    if (!op.empty()) {
      // use the operator string to represent the operator
      assert(entry.children_.size() == 2);
      return entry.children_[0]->ToString() + " " + op + " " + entry.children_[1]->ToString();
    }
    switch (entry.type_) {
      case ExpressionType::COMPARE_IN:
      case ExpressionType::COMPARE_NOT_IN: {
        string op_type = entry.type_ == ExpressionType::COMPARE_IN ? " IN " : " NOT IN ";
        string in_child = entry.children_[0]->ToString();
        string child_list = "(";
        for (uint64_t i = 1; i < entry.children_.size(); i++) {
          if (i > 1) {
            child_list += ", ";
          }
          child_list += entry.children_[i]->ToString();
        }
        child_list += ")";
        return "(" + in_child + op_type + child_list + ")";
      }
      case ExpressionType::OPERATOR_NOT:
      case ExpressionType::GROUPING_FUNCTION:
      case ExpressionType::OPERATOR_COALESCE: {
        string result = ExpressionTypeToString(entry.type_);
        result += "(";
        result += StringUtil::Join(entry.children_, entry.children_.size(), ", ",
                                   [](const unique_ptr<BASE> &child) { return child->ToString(); });
        result += ")";
        return result;
      }
      case ExpressionType::OPERATOR_IS_NULL:
        return "(" + entry.children_[0]->ToString() + " IS NULL)";
      case ExpressionType::OPERATOR_IS_NOT_NULL:
        return "(" + entry.children_[0]->ToString() + " IS NOT NULL)";
      case ExpressionType::ARRAY_EXTRACT:
        return entry.children_[0]->ToString() + "[" + entry.children_[1]->ToString() + "]";
      case ExpressionType::ARRAY_SLICE:
        return entry.children_[0]->ToString() + "[" + entry.children_[1]->ToString() + ":" +
               entry.children_[2]->ToString() + "]";
      case ExpressionType::STRUCT_EXTRACT: {
        assert(entry.children_[1]->type_ == ExpressionType::VALUE_CONSTANT);
        auto child_string = entry.children_[1]->ToString();
        assert(child_string.size() >= 3);
        assert(child_string[0] == '\'' && child_string[child_string.size() - 1] == '\'');
        return "(" + entry.children_[0]->ToString() + ")." +
               KeywordHelper::WriteOptionallyQuoted(child_string.substr(1, child_string.size() - 2));
      }
      case ExpressionType::ARRAY_CONSTRUCTOR: {
        string result = "(ARRAY[";
        result += StringUtil::Join(entry.children_, entry.children_.size(), ", ",
                                   [](const unique_ptr<BASE> &child) { return child->ToString(); });
        result += "])";
        return result;
      }
      default:
        throw Exception("Unrecognized operator type");
    }
  }
};

}  // namespace bustub
