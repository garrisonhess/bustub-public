#include "catalog/catalog.h"
#include "catalog/catalog_entry/pragma_function_catalog_entry.h"
#include "parser/statement/pragma_statement.h"
#include "planner/binder.h"
#include "planner/operator/logical_pragma.h"

namespace bustub {

BoundStatement Binder::Bind(PragmaStatement &stmt) {
  auto &catalog = Catalog::GetCatalog(context);
  // bind the pragma function
  auto entry = catalog.GetEntry<PragmaFunctionCatalogEntry>(context, DEFAULT_SCHEMA, stmt.info->name, false);
  string error;
  uint64_t bound_idx = Function::BindFunction(entry->name, entry->functions, *stmt.info, error);
  if (bound_idx == DConstants::INVALID_INDEX) {
    throw BinderException(FormatError(stmt.stmt_location, error));
  }
  auto &bound_function = entry->functions[bound_idx];
  if (!bound_function.function) {
    throw BinderException("PRAGMA function does not have a function specified");
  }

  // bind and check named params
  QueryErrorContext error_context(root_statement, stmt.stmt_location);
  BindNamedParameters(bound_function.named_parameters, stmt.info->named_parameters, error_context, bound_function.name);

  BoundStatement result;
  result.names = {"Success"};
  result.types = {LogicalType::BOOLEAN};
  result.plan = make_unique<LogicalPragma>(bound_function, *stmt.info);
  properties.return_type = StatementReturnType::QUERY_RESULT;
  return result;
}

}  // namespace bustub
