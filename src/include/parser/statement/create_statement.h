
#pragma once

#include "catalog/column.h"
#include "common/logger.h"
#include "nodes/parsenodes.hpp"
#include "parser/parser.h"
#include "parser/sql_statement.h"
#include "parser/statement/create_statement.h"

namespace bustub {

class CreateStatement : public SQLStatement {
 public:
  explicit CreateStatement(Parser &parser, bustub_libpgquery::PGCreateStmt *pg_stmt);

  string table_;
  vector<Column> columns_;
};

}  // namespace bustub
