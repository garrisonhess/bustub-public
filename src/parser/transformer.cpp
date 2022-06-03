#include "parser/transformer.h"

#include "common/logger.h"
#include "parser/sql_statement.h"

namespace bustub {

Transformer::Transformer(Transformer *parent) : parent_(parent) {}

bool Transformer::TransformParseTree(bustub_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements) {
  for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
    auto stmt = TransformStatement(static_cast<bustub_libpgquery::PGNode *>(entry->data.ptr_value));
    statements.push_back(move(stmt));
  }
  return true;
}

unique_ptr<SQLStatement> Transformer::TransformStatement(bustub_libpgquery::PGNode *stmt) {
  switch (stmt->type) {
    case bustub_libpgquery::T_PGRawStmt: {
      auto raw_stmt = reinterpret_cast<bustub_libpgquery::PGRawStmt *>(stmt);
      auto result = TransformStatement(raw_stmt->stmt);
      if (result) {
        result->stmt_location_ = raw_stmt->stmt_location;
        result->stmt_length_ = raw_stmt->stmt_len;
      }
      return result;
    }
    case bustub_libpgquery::T_PGSelectStmt:
      // return TransformSelect(stmt);
    case bustub_libpgquery::T_PGCreateStmt:
    // {
    //   auto tmp = TransformCreateTable(stmt);
    //   return tmp;
    // }
    case bustub_libpgquery::T_PGInsertStmt:
    // {
    //   auto tmp2 = TransformInsert(stmt);
    //   return tmp2;
    // }
    case bustub_libpgquery::T_PGUpdateStmt:
      // return TransformUpdate(stmt);
    case bustub_libpgquery::T_PGDeleteStmt:
      // return TransformDelete(stmt);
    case bustub_libpgquery::T_PGIndexStmt:
      // return TransformCreateIndex(stmt);
    case bustub_libpgquery::T_PGCreateSchemaStmt:
    case bustub_libpgquery::T_PGViewStmt:
    case bustub_libpgquery::T_PGCreateSeqStmt:
    case bustub_libpgquery::T_PGCreateFunctionStmt:
    case bustub_libpgquery::T_PGDropStmt:
    case bustub_libpgquery::T_PGCopyStmt:
    case bustub_libpgquery::T_PGTransactionStmt:
    case bustub_libpgquery::T_PGAlterTableStmt:
    case bustub_libpgquery::T_PGRenameStmt:
    case bustub_libpgquery::T_PGPrepareStmt:
    case bustub_libpgquery::T_PGExecuteStmt:
    case bustub_libpgquery::T_PGDeallocateStmt:
    case bustub_libpgquery::T_PGCreateTableAsStmt:
    case bustub_libpgquery::T_PGPragmaStmt:
    case bustub_libpgquery::T_PGExportStmt:
    case bustub_libpgquery::T_PGImportStmt:
    case bustub_libpgquery::T_PGExplainStmt:
    case bustub_libpgquery::T_PGVacuumStmt:
    case bustub_libpgquery::T_PGVariableShowStmt:
    case bustub_libpgquery::T_PGVariableShowSelectStmt:
    case bustub_libpgquery::T_PGCallStmt:
    case bustub_libpgquery::T_PGVariableSetStmt:
    case bustub_libpgquery::T_PGCheckPointStmt:
    case bustub_libpgquery::T_PGLoadStmt:
    case bustub_libpgquery::T_PGCreateEnumStmt:
    case bustub_libpgquery::T_PGAlterSeqStmt:
      throw NotImplementedException(NodetypeToString(stmt->type));
    default:
      throw NotImplementedException(NodetypeToString(stmt->type));
  }

  throw NotImplementedException(NodetypeToString(stmt->type));
}

}  // namespace bustub
