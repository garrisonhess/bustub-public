//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/execute_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/vector.h"
#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

class ExecuteStatement : public SQLStatement {
 public:
  ExecuteStatement();

  string name;
  vector<unique_ptr<ParsedExpression>> values;

 protected:
  ExecuteStatement(const ExecuteStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};
}  // namespace bustub
