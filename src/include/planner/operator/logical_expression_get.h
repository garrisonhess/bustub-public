//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_expression_get.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <utility>
#include "planner/logical_operator.h"
#include "type/type.h"

namespace bustub {

//! LogicalExpressionGet represents a scan operation over a set of to-be-executed expressions
class LogicalExpressionGet : public LogicalOperator {
 public:
  LogicalExpressionGet(uint64_t table_index, std::vector<Type> types,
                       vector<vector<unique_ptr<Expression>>> expressions)
      : LogicalOperator(LogicalOperatorType::LOGICAL_EXPRESSION_GET),
        table_index_(table_index),
        expr_types_(std::move(types)),
        expressions_(move(expressions)) {}

  //! The table index in the current bind context
  uint64_t table_index_;
  //! The types of the expressions
  vector<Type> expr_types_;
  //! The set of expressions
  vector<vector<unique_ptr<Expression>>> expressions_;

 public:
  vector<ColumnBinding> GetColumnBindings() override {
    return GenerateColumnBindings(table_index_, expr_types_.size());
  }

 protected:
  void ResolveTypes() override {
    // types are resolved in the constructor
    this->types_ = expr_types_;
  }
};
}  // namespace bustub
