
#pragma once

#include <string>

#include "parser/parser.h"

namespace bustub {

class DeleteStatement : public SQLStatement {
 public:
  explicit DeleteStatement(Parser &parser, bustub_libpgquery::PGDeleteStmt *pg_stmt);

  string table_;
};

}  // namespace bustub
