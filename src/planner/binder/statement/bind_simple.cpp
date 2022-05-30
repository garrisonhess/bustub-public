#include "catalog/catalog.h"
#include "catalog/catalog_entry/table_catalog_entry.h"
#include "catalog/catalog_entry/view_catalog_entry.h"
#include "parser/statement/alter_statement.h"
#include "parser/statement/transaction_statement.h"
#include "planner/binder.h"
#include "planner/operator/logical_simple.h"

//! This file contains the binder definitions for statements that do not need to be bound at all and only require a
//! straightforward conversion

namespace bustub {

BoundStatement Binder::Bind(AlterStatement &stmt) {
  BoundStatement result;
  result.names = {"Success"};
  result.types = {LogicalType::BOOLEAN};
  Catalog &catalog = Catalog::GetCatalog(context);
  auto entry = catalog.GetEntry(context, stmt.info->GetCatalogType(), stmt.info->schema, stmt.info->name, true);
  if (entry && !entry->temporary) {
    // we can only alter temporary tables/views in read-only mode
    properties.read_only = false;
  }
  result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_ALTER, move(stmt.info));
  properties.return_type = StatementReturnType::NOTHING;
  return result;
}

BoundStatement Binder::Bind(TransactionStatement &stmt) {
  // transaction statements do not require a valid transaction
  properties.requires_valid_transaction = false;

  BoundStatement result;
  result.names = {"Success"};
  result.types = {LogicalType::BOOLEAN};
  result.plan = make_unique<LogicalSimple>(LogicalOperatorType::LOGICAL_TRANSACTION, move(stmt.info));
  properties.return_type = StatementReturnType::NOTHING;
  return result;
}

}  // namespace bustub
