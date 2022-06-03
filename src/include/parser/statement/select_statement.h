
#pragma once

#include "catalog/column.h"
#include "parser/sql_statement.h"

namespace bustub {

class SelectStatement : public SQLStatement {
 public:
  SelectStatement();

  string table_;
  vector<Column> columns_;
};

}  // namespace bustub
