#include "parser/statement/delete_statement.h"

namespace bustub {

DeleteStatement::DeleteStatement(Parser &parser, bustub_libpgquery::PGDeleteStmt *pg_stmt)
    : SQLStatement(StatementType::DELETE_STATEMENT) {}

}  // namespace bustub
