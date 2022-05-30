//===----------------------------------------------------------------------===//
//                         BusTub
//
// parser/expression_map.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "parser/base_expression.h"

namespace bustub {
class Expression;

struct ExpressionHashFunction {
  uint64_t operator()(const BaseExpression *const &expr) const { return static_cast<uint64_t>(expr->Hash()); }
};

struct ExpressionEquality {
  bool operator()(const BaseExpression *const &a, const BaseExpression *const &b) const { return a->Equals(b); }
};

template <typename T>
using expression_map_t = std::unordered_map<BaseExpression *, T, ExpressionHashFunction, ExpressionEquality>;

using expression_set_t = std::unordered_set<BaseExpression *, ExpressionHashFunction, ExpressionEquality>;

}  // namespace bustub
