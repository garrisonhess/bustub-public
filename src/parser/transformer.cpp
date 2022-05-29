#include "parser/transformer.h"

#include "common/logger.h"
#include "parser/sql_statement.h"
#include "parser/statement/select_statement.h"
#include "parser/tableref/emptytableref.h"

namespace bustub {

Transformer::Transformer(Transformer *parent) : parent_(parent) {}

bool Transformer::TransformParseTree(bustub_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements) {
  for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
    SetParamCount(0);
    auto stmt = TransformStatement(static_cast<bustub_libpgquery::PGNode *>(entry->data.ptr_value));
    stmt->n_param_ = ParamCount();
    statements.push_back(move(stmt));
  }
  return true;
}

unique_ptr<SQLStatement> Transformer::TransformStatement(bustub_libpgquery::PGNode *stmt) {
  auto result = TransformStatementInternal(stmt);
  result->n_param_ = ParamCount();
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
    case bustub_libpgquery::T_PGSelectStmt: {
      unique_ptr<SelectStatement> x = TransformSelect(stmt);
      return x;
    }
    case bustub_libpgquery::T_PGCreateStmt:
      // return TransformCreateTable(stmt);
      break;
    case bustub_libpgquery::T_PGCreateSchemaStmt:
      // return TransformCreateSchema(stmt);
      break;
    case bustub_libpgquery::T_PGViewStmt:
      // return TransformCreateView(stmt);
      break;
    case bustub_libpgquery::T_PGCreateSeqStmt:
      // return TransformCreateSequence(stmt);
      break;
    case bustub_libpgquery::T_PGCreateFunctionStmt:
      // return TransformCreateFunction(stmt);
      break;
    case bustub_libpgquery::T_PGDropStmt:
      // return TransformDrop(stmt);
      break;
    case bustub_libpgquery::T_PGInsertStmt:
      // return TransformInsert(stmt);
      break;
    case bustub_libpgquery::T_PGCopyStmt:
      // return TransformCopy(stmt);
      break;
    case bustub_libpgquery::T_PGTransactionStmt:
      // return TransformTransaction(stmt);
      break;
    case bustub_libpgquery::T_PGDeleteStmt:
      // return TransformDelete(stmt);
      break;
    case bustub_libpgquery::T_PGUpdateStmt:
      // return TransformUpdate(stmt);
      break;
    case bustub_libpgquery::T_PGIndexStmt:
      // return TransformCreateIndex(stmt);
      break;
    case bustub_libpgquery::T_PGAlterTableStmt:
      // return TransformAlter(stmt);
      break;
    case bustub_libpgquery::T_PGRenameStmt:
      // return TransformRename(stmt);
      break;
    case bustub_libpgquery::T_PGPrepareStmt:
      // return TransformPrepare(stmt);
      break;
    case bustub_libpgquery::T_PGExecuteStmt:
      // return TransformExecute(stmt);
      break;
    case bustub_libpgquery::T_PGDeallocateStmt:
      // return TransformDeallocate(stmt);
      break;
    case bustub_libpgquery::T_PGCreateTableAsStmt:
      // return TransformCreateTableAs(stmt);
      break;
    case bustub_libpgquery::T_PGPragmaStmt:
      // return TransformPragma(stmt);
      break;
    case bustub_libpgquery::T_PGExportStmt:
      // return TransformExport(stmt);
      break;
    case bustub_libpgquery::T_PGImportStmt:
      // return TransformImport(stmt);
      break;
    case bustub_libpgquery::T_PGExplainStmt:
      // return TransformExplain(stmt);
      break;
    case bustub_libpgquery::T_PGVacuumStmt:
      // return TransformVacuum(stmt);
      break;
    case bustub_libpgquery::T_PGVariableShowStmt:
      // return TransformShow(stmt);
      break;
    case bustub_libpgquery::T_PGVariableShowSelectStmt:
      // return TransformShowSelect(stmt);
    case bustub_libpgquery::T_PGCallStmt:
      // return TransformCall(stmt);
      break;
    case bustub_libpgquery::T_PGVariableSetStmt:
      // return TransformSet(stmt);
      break;
    case bustub_libpgquery::T_PGCheckPointStmt:
      // return TransformCheckpoint(stmt);
      break;
    case bustub_libpgquery::T_PGLoadStmt:
      // return TransformLoad(stmt);
      break;
    case bustub_libpgquery::T_PGCreateEnumStmt:
      // return TransformCreateEnum(stmt);
      break;
    case bustub_libpgquery::T_PGAlterSeqStmt:
      break;
      // return TransformAlterSequence(stmt);
    default:
      throw NotImplementedException(NodetypeToString(stmt->type));
  }
  return nullptr;
}

}  // namespace bustub
