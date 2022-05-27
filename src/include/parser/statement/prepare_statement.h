//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/prepare_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

class PrepareStatement : public SQLStatement {
 public:
  PrepareStatement();

  unique_ptr<SQLStatement> statement;
  string name;

 protected:
  PrepareStatement(const PrepareStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};
}  // namespace bustub
