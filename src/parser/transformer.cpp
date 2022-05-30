#include "parser/transformer.h"

#include "common/logger.h"
#include "parser/sql_statement.h"
#include "parser/statement/create_statement.h"
#include "parser/statement/insert_statement.h"
#include "parser/statement/select_statement.h"
#include "parser/tableref/emptytableref.h"

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
  auto result = TransformStatementInternal(stmt);
  return result;
}

unique_ptr<SQLStatement> Transformer::TransformStatementInternal(bustub_libpgquery::PGNode *stmt) {
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
      return TransformSelect(stmt);
    case bustub_libpgquery::T_PGCreateStmt: {
      auto tmp = TransformCreateTable(stmt);
      return tmp;
    }
    case bustub_libpgquery::T_PGInsertStmt: {
      auto tmp2 = TransformInsert(stmt);
      return tmp2;
    }
    case bustub_libpgquery::T_PGUpdateStmt:
      // return TransformUpdate(stmt);
    case bustub_libpgquery::T_PGDeleteStmt:
      // return TransformDelete(stmt);
    case bustub_libpgquery::T_PGIndexStmt:
      // return TransformCreateIndex(stmt);
    case bustub_libpgquery::T_PGCreateSchemaStmt:
      // return TransformCreateSchema(stmt);
    case bustub_libpgquery::T_PGViewStmt:
      // return TransformCreateView(stmt);
    case bustub_libpgquery::T_PGCreateSeqStmt:
      // return TransformCreateSequence(stmt);
    case bustub_libpgquery::T_PGCreateFunctionStmt:
      // return TransformCreateFunction(stmt);
    case bustub_libpgquery::T_PGDropStmt:
      // return TransformDrop(stmt);
    case bustub_libpgquery::T_PGCopyStmt:
      // return TransformCopy(stmt);
    case bustub_libpgquery::T_PGTransactionStmt:
      // return TransformTransaction(stmt);
    case bustub_libpgquery::T_PGAlterTableStmt:
      // return TransformAlter(stmt);
    case bustub_libpgquery::T_PGRenameStmt:
      // return TransformRename(stmt);
    case bustub_libpgquery::T_PGPrepareStmt:
      // return TransformPrepare(stmt);
    case bustub_libpgquery::T_PGExecuteStmt:
      // return TransformExecute(stmt);
    case bustub_libpgquery::T_PGDeallocateStmt:
      // return TransformDeallocate(stmt);
    case bustub_libpgquery::T_PGCreateTableAsStmt:
      // return TransformCreateTableAs(stmt);
    case bustub_libpgquery::T_PGPragmaStmt:
      // return TransformPragma(stmt);
    case bustub_libpgquery::T_PGExportStmt:
      // return TransformExport(stmt);
    case bustub_libpgquery::T_PGImportStmt:
      // return TransformImport(stmt);
    case bustub_libpgquery::T_PGExplainStmt:
      // return TransformExplain(stmt);
    case bustub_libpgquery::T_PGVacuumStmt:
      // return TransformVacuum(stmt);
    case bustub_libpgquery::T_PGVariableShowStmt:
      // return TransformShow(stmt);
    case bustub_libpgquery::T_PGVariableShowSelectStmt:
      // return TransformShowSelect(stmt);
    case bustub_libpgquery::T_PGCallStmt:
      // return TransformCall(stmt);
    case bustub_libpgquery::T_PGVariableSetStmt:
      // return TransformSet(stmt);
    case bustub_libpgquery::T_PGCheckPointStmt:
      // return TransformCheckpoint(stmt);
    case bustub_libpgquery::T_PGLoadStmt:
      // return TransformLoad(stmt);
    case bustub_libpgquery::T_PGCreateEnumStmt:
      // return TransformCreateEnum(stmt);
    case bustub_libpgquery::T_PGAlterSeqStmt:
      // return TransformAlterSequence(stmt);
      throw NotImplementedException(NodetypeToString(stmt->type));
    default:
      throw NotImplementedException(NodetypeToString(stmt->type));
  }

  throw NotImplementedException(NodetypeToString(stmt->type));
}

}  // namespace bustub
