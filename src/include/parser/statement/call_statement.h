//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/call_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/vector.h"
#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

class CallStatement : public SQLStatement {
 public:
  CallStatement();

  unique_ptr<ParsedExpression> function;

 protected:
  CallStatement(const CallStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};
}  // namespace bustub
