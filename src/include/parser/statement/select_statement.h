
#pragma once

#include "parser/parser.h"

namespace bustub {

class ASelectStatement : public SQLStatement {
 public:
  explicit ASelectStatement(Parser &parser, bustub_libpgquery::PGSelectStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
};

}  // namespace bustub
