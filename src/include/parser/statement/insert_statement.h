
#pragma once

#include "parser/parser.h"

#include <string>
#include <vector>

namespace bustub {

class InsertStatement : public SQLStatement {
 public:
  explicit InsertStatement(Parser &parser, bustub_libpgquery::PGInsertStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
  vector<vector<Value>> values_;
};

}  // namespace bustub
