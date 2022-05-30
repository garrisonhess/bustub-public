#include "planner/pragma_handler.h"
#include "parser/parser.h"
#include "planner/binder.h"

#include "catalog/catalog.h"
#include "catalog/catalog_entry/pragma_function_catalog_entry.h"

#include "function/function.h"
#include "parser/parsed_data/pragma_info.h"

#include "main/client_context.h"

#include "common/file_system.h"
#include "common/string_util.h"

namespace bustub {

PragmaHandler::PragmaHandler(ClientContext &context) : context(context) {}

void PragmaHandler::HandlePragmaStatementsInternal(vector<unique_ptr<SQLStatement>> &statements) {
  vector<unique_ptr<SQLStatement>> new_statements;
  for (uint64_t i = 0; i < statements.size(); i++) {
    if (statements[i]->type == StatementType::PRAGMA_STATEMENT) {
      // PRAGMA statement: check if we need to replace it by a new set of statements
      PragmaHandler handler(context);
      auto new_query = handler.HandlePragma(statements[i].get());  //*((PragmaStatement &)*statements[i]).info
      if (!new_query.empty()) {
        // this PRAGMA statement gets replaced by a new query string
        // push the new query string through the parser again and add it to the transformer
        Parser parser(context.GetParserOptions());
        parser.ParseQuery(new_query);
        // insert the new statements and remove the old statement
        // FIXME: off by one here maybe?
        for (uint64_t j = 0; j < parser.statements.size(); j++) {
          new_statements.push_back(move(parser.statements[j]));
        }
        continue;
      }
    }
    new_statements.push_back(move(statements[i]));
  }
  statements = move(new_statements);
}

void PragmaHandler::HandlePragmaStatements(ClientContextLock &lock, vector<unique_ptr<SQLStatement>> &statements) {
  // first check if there are any pragma statements
  bool found_pragma = false;
  for (uint64_t i = 0; i < statements.size(); i++) {
    if (statements[i]->type == StatementType::PRAGMA_STATEMENT) {
      found_pragma = true;
      break;
    }
  }
  if (!found_pragma) {
    // no pragmas: skip this step
    return;
  }
  context.RunFunctionInTransactionInternal(lock, [&]() { HandlePragmaStatementsInternal(statements); });
}

string PragmaHandler::HandlePragma(SQLStatement *statement) {  // PragmaInfo &info
  auto info = *((PragmaStatement &)*statement).info;
  auto entry =
      Catalog::GetCatalog(context).GetEntry<PragmaFunctionCatalogEntry>(context, DEFAULT_SCHEMA, info.name, false);
  string error;
  uint64_t bound_idx = Function::BindFunction(entry->name, entry->functions, info, error);
  if (bound_idx == DConstants::INVALID_INDEX) {
    throw BinderException(error);
  }
  auto &bound_function = entry->functions[bound_idx];
  if (bound_function.query) {
    QueryErrorContext error_context(statement, statement->stmt_location);
    Binder::BindNamedParameters(bound_function.named_parameters, info.named_parameters, error_context,
                                bound_function.name);
    FunctionParameters parameters{info.parameters, info.named_parameters};
    return bound_function.query(context, parameters);
  }
  return string();
}

}  // namespace bustub
