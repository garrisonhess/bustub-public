
#pragma once

#include <string>
#include <vector>

#include "parser/parser.h"

namespace bustub {

class CreateStatement : public SQLStatement {
 public:
  explicit CreateStatement(Parser &parser, bustub_libpgquery::PGCreateStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
};

}  // namespace bustub
