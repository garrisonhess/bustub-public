#include "parser/statement/create_statement.h"
#include "parser/transformer.h"

#include <memory>

namespace bustub {

ColumnDefinition Transformer::TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef) {
  string colname;
  if (cdef->colname != nullptr) {
    colname = cdef->colname;
  }
  Type target_type = TransformTypeName(cdef->typeName);
  return ColumnDefinition(colname, target_type);
}

unique_ptr<CreateStatement> Transformer::TransformCreateTable(bustub_libpgquery::PGNode *node) {
  auto stmt = reinterpret_cast<bustub_libpgquery::PGCreateStmt *>(node);
  assert(stmt);
  auto result = std::make_unique<CreateStatement>();

  if (stmt->inhRelations != nullptr) {
    throw NotImplementedException("inherited relations not implemented");
  }

  result->schema_ = INVALID_SCHEMA;
  if (stmt->relation->schemaname != nullptr) {
    result->schema_ = stmt->relation->schemaname;
  }
  result->table_ = stmt->relation->relname;

  if (stmt->oncommit != bustub_libpgquery::PGOnCommitAction::PG_ONCOMMIT_PRESERVE_ROWS &&
      stmt->oncommit != bustub_libpgquery::PGOnCommitAction::PG_ONCOMMIT_NOOP) {
    throw NotImplementedException("Only ON COMMIT PRESERVE ROWS is supported");
  }
  if (stmt->tableElts == nullptr) {
    throw Exception("Table must have at least one column!");
  }

  for (auto c = stmt->tableElts->head; c != nullptr; c = lnext(c)) {
    auto node = reinterpret_cast<bustub_libpgquery::PGNode *>(c->data.ptr_value);
    switch (node->type) {
      case bustub_libpgquery::T_PGColumnDef: {
        auto cdef = static_cast<bustub_libpgquery::PGColumnDef *>(c->data.ptr_value);
        auto centry = TransformColumnDefinition(cdef);
        result->columns_.push_back(std::move(centry));
        break;
      }
      default:
        throw NotImplementedException("ColumnDef type not handled yet");
    }
  }

  return result;
}

}  // namespace bustub
