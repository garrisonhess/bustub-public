
#pragma once

#include "parser/parser.h"

#include <string>
#include <vector>

namespace bustub {

class SelectStatement : public SQLStatement {
 public:
  explicit SelectStatement(Parser &parser, bustub_libpgquery::PGSelectStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
};

}  // namespace bustub
