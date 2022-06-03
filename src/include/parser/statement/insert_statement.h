
#pragma once

#include "execution/expressions/constant_value_expression.h"
#include "parser/parser.h"

namespace bustub {

class InsertStatement : public SQLStatement {
 public:
  explicit InsertStatement(Parser &parser, bustub_libpgquery::PGInsertStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
  vector<vector<ConstantValueExpression>> values_;
};

}  // namespace bustub
