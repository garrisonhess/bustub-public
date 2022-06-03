#include "parser/statement/insert_statement.h"

namespace bustub {

InsertStatement::InsertStatement(Parser &parser, bustub_libpgquery::PGInsertStmt *pg_stmt)
    : SQLStatement(StatementType::INSERT_STATEMENT) {
  table_ = pg_stmt->relation->relname;
}

}  // namespace bustub
