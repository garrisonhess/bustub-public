
#pragma once

#include <string>
#include <vector>

#include "parser/parser.h"

namespace bustub {

class InsertStatement : public SQLStatement {
 public:
  explicit InsertStatement(Parser &parser, bustub_libpgquery::PGInsertStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
  vector<vector<Value>> values_;
};

}  // namespace bustub
