
#pragma once

#include "catalog/column.h"
#include "execution/expressions/constant_value_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

class InsertStatement : public SQLStatement {
 public:
  InsertStatement();

  string table_;
  vector<Column> columns_;
  vector<vector<ConstantValueExpression>> values_;
};

}  // namespace bustub
