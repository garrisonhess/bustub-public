#include "planner/planner.h"
#include "common/exception.h"
#include "common/serializer.h"
// #include "execution/expression_executor.h"
#include "main/client_context.h"
// #include "main/client_data.h"
#include "main/database.h"
#include "planner/binder.h"
#include "planner/expression_binder/constant_binder.h"
// #include "transaction/transaction.h"

namespace bustub {

Planner::Planner(ClientContext &context) : binder_(Binder::CreateBinder(context)), context_(context) {}

void Planner::CreatePlan(SQLStatement &statement) {
  LOG_INFO("In planner CreatePlan, about to Bind");
  auto bound_statement = binder_->Bind(statement);
  names_ = bound_statement.names_;
  types_ = bound_statement.types_;

  LOG_INFO("bound statment has %zu names", bound_statement.names_.size());
  for (auto &name : bound_statement.names_) {
    LOG_INFO("name: %s", name.c_str());
  }

  LOG_INFO("bound statment has %zu types_", bound_statement.types_.size());
  plan_ = move(bound_statement.plan_);
}

shared_ptr<PreparedStatementData> Planner::PrepareSQLStatement(unique_ptr<SQLStatement> statement) {
  // auto copied_statement = statement->Copy();
  // create a plan of the underlying statement
  // CreatePlan(move(statement));
  // // now create the logical prepare
  // auto prepared_data = make_shared<PreparedStatementData>(copied_statement->type);
  // prepared_data->unbound_statement = move(copied_statement);
  // prepared_data->names = names;
  // prepared_data->types = types;
  // prepared_data->value_map = move(value_map);
  // prepared_data->properties = properties;
  // prepared_data->catalog_version = Transaction::GetTransaction(context).catalog_version;
  // return prepared_data;
  throw NotImplementedException("preparesql");
}

// void Planner::PlanExecute(unique_ptr<SQLStatement> statement) {
//   auto &stmt = (ExecuteStatement &)*statement;

//   // bind the prepared statement
//   auto &client_data = ClientData::Get(context);

//   auto entry = client_data.prepared_statements.find(stmt.name);
//   if (entry == client_data.prepared_statements.end()) {
//     throw Exception("Prepared statement \"%s\" does not exist", stmt.name);
//   }

//   // check if we need to rebind the prepared statement
//   // this happens if the catalog changes, since in this case e.g. tables we relied on may have been deleted
//   auto prepared = entry->second;
//   auto &catalog = Catalog::GetCatalog(context);
//   bool rebound = false;

//   // bind any supplied parameters
//   vector<Value> bind_values;
//   for (uint64_t i = 0; i < stmt.values.size(); i++) {
//     ConstantBinder cbinder(*binder, context, "EXECUTE statement");
//     auto bound_expr = cbinder.Bind(stmt.values[i]);

//     Value value = ExpressionExecutor::EvaluateScalar(*bound_expr);
//     bind_values.push_back(move(value));
//   }
//   bool all_bound = prepared->properties.bound_all_parameters;
//   if (catalog.GetCatalogVersion() != entry->second->catalog_version || !all_bound) {
//     // catalog was modified or statement does not have clear types: rebind the statement before running the execute
//     for (auto &value : bind_values) {
//       parameter_types.push_back(value.type());
//     }
//     prepared = PrepareSQLStatement(entry->second->unbound_statement->Copy());
//     if (all_bound && prepared->types != entry->second->types) {
//       throw Exception("Rebinding statement \"%s\" after catalog change resulted in change of types", stmt.name);
//     }
//     assert(prepared->properties.bound_all_parameters);
//     rebound = true;
//   }
//   // add casts to the prepared statement parameters as required
//   for (uint64_t i = 0; i < bind_values.size(); i++) {
//     if (prepared->value_map.count(i + 1) == 0) {
//       continue;
//     }
//     bind_values[i] = bind_values[i].CastAs(prepared->GetType(i + 1));
//   }

//   prepared->Bind(move(bind_values));
//   if (rebound) {
//     auto execute_plan = make_unique<LogicalExecute>(move(prepared));
//     execute_plan->children.push_back(move(plan));
//     this->plan = move(execute_plan);
//     return;
//   }

//   // copy the properties of the prepared statement into the planner
//   this->properties = prepared->properties;
//   this->names = prepared->names;
//   this->types = prepared->types;
//   this->plan = make_unique<LogicalExecute>(move(prepared));
// }

// void Planner::PlanPrepare(unique_ptr<SQLStatement> statement) {
//   auto &stmt = (PrepareStatement &)*statement;
//   auto prepared_data = PrepareSQLStatement(move(stmt.statement));

//   auto prepare = make_unique<LogicalPrepare>(stmt.name, move(prepared_data), move(plan));
//   // we can prepare in read-only mode: prepared statements are not written to the catalog
//   properties.read_only = true;
//   // we can always prepare, even if the transaction has been invalidated
//   // this is required because most clients ALWAYS invoke prepared statements
//   properties.requires_valid_transaction = false;
//   properties.allow_stream_result = false;
//   properties.bound_all_parameters = true;
//   properties.return_type = StatementReturnType::NOTHING;
//   this->names = {"Success"};
//   this->types = {Type(TypeId::BOOLEAN)};
//   this->plan = move(prepare);
// }

void Planner::CreatePlan(unique_ptr<SQLStatement> statement) {
  assert(statement);
  switch (statement->type_) {
    case StatementType::SELECT_STATEMENT:
    case StatementType::INSERT_STATEMENT:
    case StatementType::COPY_STATEMENT:
    case StatementType::DELETE_STATEMENT:
    case StatementType::UPDATE_STATEMENT:
    case StatementType::CREATE_STATEMENT:
    case StatementType::DROP_STATEMENT:
    case StatementType::ALTER_STATEMENT:
    case StatementType::TRANSACTION_STATEMENT:
    case StatementType::EXPLAIN_STATEMENT:
    case StatementType::VACUUM_STATEMENT:
    case StatementType::RELATION_STATEMENT:
    case StatementType::CALL_STATEMENT:
    case StatementType::EXPORT_STATEMENT:
    case StatementType::PRAGMA_STATEMENT:
    case StatementType::SHOW_STATEMENT:
    case StatementType::SET_STATEMENT:
    case StatementType::LOAD_STATEMENT:
      LOG_INFO("Createplan for statement: %s", statement->ToString().c_str());
      CreatePlan(*statement);
      break;
    case StatementType::EXECUTE_STATEMENT:
    case StatementType::PREPARE_STATEMENT:
      throw NotImplementedException("CreatePlan for PREPARE/EXECUTE are not implemented!");
    default:
      throw NotImplementedException("Cannot plan statement of type X!");
  }
}

}  // namespace bustub
