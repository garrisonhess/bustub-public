#include "planner/planner.h"
#include "common/serializer.h"
// #include "execution/expression_executor.h"
#include "main/client_context.h"
// #include "main/client_data.h"
#include "main/database.h"
#include "planner/binder.h"
#include "planner/expression_binder/constant_binder.h"
// #include "transaction/transaction.h"

namespace bustub {

Planner::Planner(ClientContext &context) : binder(Binder::CreateBinder(context)), context(context) {}

void Planner::CreatePlan(SQLStatement &statement) {
  auto &profiler = QueryProfiler::Get(context);

  vector<BoundParameterExpression *> bound_parameters;

  // first bind the tables and columns to the catalog
  profiler.StartPhase("binder");
  binder->parameters = &bound_parameters;
  binder->parameter_types = &parameter_types;
  auto bound_statement = binder->Bind(statement);
  profiler.EndPhase();

  this->properties = binder->properties;
  this->names = bound_statement.names;
  this->types = bound_statement.types;
  this->plan = move(bound_statement.plan);
  properties.bound_all_parameters = true;

  // set up a map of parameter number -> value entries
  for (auto &expr : bound_parameters) {
    // check if the type of the parameter could be resolved
    if (expr->return_type.id() == TypeId::INVALID || expr->return_type.id() == TypeId::UNKNOWN) {
      properties.bound_all_parameters = false;
      continue;
    }
    auto value = make_unique<Value>(expr->return_type);
    expr->value = value.get();
    // check if the parameter number has been used before
    auto entry = value_map.find(expr->parameter_nr);
    if (entry == value_map.end()) {
      // not used before, create vector
      value_map[expr->parameter_nr] = vector<unique_ptr<Value>>();
    } else if (entry->second.back()->type() != value->type()) {
      // used before, but types are inconsistent
      throw BinderException("Inconsistent types found for parameter with index %llu", expr->parameter_nr);
    }
    value_map[expr->parameter_nr].push_back(move(value));
  }
}

shared_ptr<PreparedStatementData> Planner::PrepareSQLStatement(unique_ptr<SQLStatement> statement) {
  auto copied_statement = statement->Copy();
  // create a plan of the underlying statement
  CreatePlan(move(statement));
  // now create the logical prepare
  auto prepared_data = make_shared<PreparedStatementData>(copied_statement->type);
  prepared_data->unbound_statement = move(copied_statement);
  prepared_data->names = names;
  prepared_data->types = types;
  prepared_data->value_map = move(value_map);
  prepared_data->properties = properties;
  prepared_data->catalog_version = Transaction::GetTransaction(context).catalog_version;
  return prepared_data;
}

void Planner::PlanExecute(unique_ptr<SQLStatement> statement) {
  auto &stmt = (ExecuteStatement &)*statement;

  // bind the prepared statement
  auto &client_data = ClientData::Get(context);

  auto entry = client_data.prepared_statements.find(stmt.name);
  if (entry == client_data.prepared_statements.end()) {
    throw BinderException("Prepared statement \"%s\" does not exist", stmt.name);
  }

  // check if we need to rebind the prepared statement
  // this happens if the catalog changes, since in this case e.g. tables we relied on may have been deleted
  auto prepared = entry->second;
  auto &catalog = Catalog::GetCatalog(context);
  bool rebound = false;

  // bind any supplied parameters
  vector<Value> bind_values;
  for (uint64_t i = 0; i < stmt.values.size(); i++) {
    ConstantBinder cbinder(*binder, context, "EXECUTE statement");
    auto bound_expr = cbinder.Bind(stmt.values[i]);

    Value value = ExpressionExecutor::EvaluateScalar(*bound_expr);
    bind_values.push_back(move(value));
  }
  bool all_bound = prepared->properties.bound_all_parameters;
  if (catalog.GetCatalogVersion() != entry->second->catalog_version || !all_bound) {
    // catalog was modified or statement does not have clear types: rebind the statement before running the execute
    for (auto &value : bind_values) {
      parameter_types.push_back(value.type());
    }
    prepared = PrepareSQLStatement(entry->second->unbound_statement->Copy());
    if (all_bound && prepared->types != entry->second->types) {
      throw BinderException("Rebinding statement \"%s\" after catalog change resulted in change of types", stmt.name);
    }
    assert(prepared->properties.bound_all_parameters);
    rebound = true;
  }
  // add casts to the prepared statement parameters as required
  for (uint64_t i = 0; i < bind_values.size(); i++) {
    if (prepared->value_map.count(i + 1) == 0) {
      continue;
    }
    bind_values[i] = bind_values[i].CastAs(prepared->GetType(i + 1));
  }

  prepared->Bind(move(bind_values));
  if (rebound) {
    auto execute_plan = make_unique<LogicalExecute>(move(prepared));
    execute_plan->children.push_back(move(plan));
    this->plan = move(execute_plan);
    return;
  }

  // copy the properties of the prepared statement into the planner
  this->properties = prepared->properties;
  this->names = prepared->names;
  this->types = prepared->types;
  this->plan = make_unique<LogicalExecute>(move(prepared));
}

void Planner::PlanPrepare(unique_ptr<SQLStatement> statement) {
  auto &stmt = (PrepareStatement &)*statement;
  auto prepared_data = PrepareSQLStatement(move(stmt.statement));

  auto prepare = make_unique<LogicalPrepare>(stmt.name, move(prepared_data), move(plan));
  // we can prepare in read-only mode: prepared statements are not written to the catalog
  properties.read_only = true;
  // we can always prepare, even if the transaction has been invalidated
  // this is required because most clients ALWAYS invoke prepared statements
  properties.requires_valid_transaction = false;
  properties.allow_stream_result = false;
  properties.bound_all_parameters = true;
  properties.return_type = StatementReturnType::NOTHING;
  this->names = {"Success"};
  this->types = {Type::BOOLEAN};
  this->plan = move(prepare);
}

void Planner::CreatePlan(unique_ptr<SQLStatement> statement) {
  assert(statement);
  switch (statement->type) {
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
      CreatePlan(*statement);
      break;
    case StatementType::EXECUTE_STATEMENT:
      PlanExecute(move(statement));
      break;
    case StatementType::PREPARE_STATEMENT:
      PlanPrepare(move(statement));
      break;
    default:
      throw NotImplementedException("Cannot plan statement of type %s!", StatementTypeToString(statement->type));
  }
}

}  // namespace bustub
