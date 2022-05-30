//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_create_index.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/create_index_info.h"
#include "planner/logical_operator.h"

namespace bustub {

class LogicalCreateIndex : public LogicalOperator {
 public:
  LogicalCreateIndex(TableCatalogEntry &table, vector<column_t> column_ids, vector<unique_ptr<Expression>> expressions,
                     unique_ptr<CreateIndexInfo> info)
      : LogicalOperator(LogicalOperatorType::LOGICAL_CREATE_INDEX),
        table(table),
        column_ids(column_ids),
        info(std::move(info)) {
    for (auto &expr : expressions) {
      this->unbound_expressions.push_back(expr->Copy());
    }
    this->expressions = move(expressions);
  }

  //! The table to create the index for
  TableCatalogEntry &table;
  //! Column IDs needed for index creation
  vector<column_t> column_ids;
  // Info for index creation
  unique_ptr<CreateIndexInfo> info;
  //! Unbound expressions to be used in the optimizer
  vector<unique_ptr<Expression>> unbound_expressions;

 protected:
  void ResolveTypes() override { types.emplace_back(Type::BIGINT); }
};
}  // namespace bustub
