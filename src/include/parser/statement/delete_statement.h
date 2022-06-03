
#pragma once

#include "catalog/column.h"
#include "parser/sql_statement.h"

namespace bustub {

class DeleteStatement : public SQLStatement {
 public:
  DeleteStatement();

  string table_;
};

}  // namespace bustub
