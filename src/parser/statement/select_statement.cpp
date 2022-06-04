#include "parser/statement/select_statement.h"

namespace bustub {

SelectStatement::SelectStatement(Parser &parser, bustub_libpgquery::PGSelectStmt *pg_stmt)
    : SQLStatement(StatementType::CREATE_STATEMENT) {
  bool found = false;

  // Extract the table name from the FROM clause.
  for (auto c = pg_stmt->fromClause->head; c != nullptr; c = lnext(c)) {
    auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(c->data.ptr_value);
    switch (node->type) {
      case bustub_libpgquery::T_PGRangeVar: {
        if (found) {
          throw Exception("shouldnt have multiple tables");
        }
        bustub_libpgquery::PGRangeVar *table_ref = reinterpret_cast<bustub_libpgquery::PGRangeVar *>(node);
        table_ = table_ref->relname;
        found = true;
        break;
      }
      default:
        throw Exception("Found unknown node type");
    }
  }
}

}  // namespace bustub
