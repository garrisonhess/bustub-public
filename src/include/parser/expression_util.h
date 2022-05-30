//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression_util.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <vector>
#include "parser/base_expression.h"

namespace bustub {
class ParsedExpression;
class Expression;
using std::unique_ptr;
using std::vector;

class ExpressionUtil {
 public:
  //! ListEquals: check if a list of two expressions is equal (order is important)
  static bool ListEquals(const vector<unique_ptr<ParsedExpression>> &a, const vector<unique_ptr<ParsedExpression>> &b);
  static bool ListEquals(const vector<unique_ptr<Expression>> &a, const vector<unique_ptr<Expression>> &b);
  //! SetEquals: check if two sets of expressions are equal (order is not important)
  static bool SetEquals(const vector<unique_ptr<ParsedExpression>> &a, const vector<unique_ptr<ParsedExpression>> &b);
  static bool SetEquals(const vector<unique_ptr<Expression>> &a, const vector<unique_ptr<Expression>> &b);

 private:
  template <class T>
  static bool ExpressionListEquals(const vector<unique_ptr<T>> &a, const vector<unique_ptr<T>> &b);
  template <class T>
  static bool ExpressionSetEquals(const vector<unique_ptr<T>> &a, const vector<unique_ptr<T>> &b);
};

}  // namespace bustub
