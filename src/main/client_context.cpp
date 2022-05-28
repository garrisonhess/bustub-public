#include "main/client_context.h"

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "common/exception.h"
#include "execution/execution_engine.h"
#include "main/database.h"
#include "main/prepared_statement_data.h"
#include "main/query_result.h"
#include "parser/parser.h"
#include "type/statement_type.h"

// #include "catalog/catalog_entry/table_catalog_entry.hpp"
// #include "common/serializer/buffered_deserializer.hpp"
// #include "common/serializer/buffered_serializer.hpp"
// #include "execution/physical_plan_generator.hpp"
// #include "main/materialized_query_result.hpp"
// #include "main/stream_query_result.hpp"
// #include "optimizer/optimizer.hpp"
// #include "parser/expression/constant_expression.hpp"
// #include "parser/statement/drop_statement.hpp"
// #include "parser/statement/execute_statement.hpp"
// #include "parser/statement/explain_statement.hpp"
// #include "parser/statement/prepare_statement.hpp"
// #include "parser/statement/select_statement.hpp"
// #include "planner/operator/logical_execute.hpp"
// #include "planner/planner.hpp"
// #include "transaction/transaction_manager.hpp"
// #include "transaction/transaction.hpp"
// #include "storage/data_table.hpp"
// #include "main/appender.hpp"
// #include "main/relation.hpp"
// #include "parser/statement/relation_statement.hpp"
// #include "parallel/task_scheduler.hpp"
// #include "common/serializer/buffered_file_writer.hpp"
// #include "planner/pragma_handler.hpp"
// #include "common/to_string.hpp"

#include <mutex>
#include <utility>

namespace bustub {
using std::lock_guard;
using std::make_unique;
using std::mutex;

ClientContext::ClientContext(shared_ptr<DatabaseInstance> database) {
  db_ = std::move(database);
  // transaction_ = TODO;
}

void ClientContext::Cleanup() {
  //   lock_guard<mutex> client_guard(context_lock_);
  //   if (is_invalidated_ || !prepared_statements_) {
  //     return;
  //   }
  //   if (transaction.HasActiveTransaction()) {
  //     ActiveTransaction().active_query = MAXIMUM_QUERY_ID;
  //     if (!transaction.IsAutoCommit()) {
  //       transaction.Rollback();
  //     }
  //   }
  // //   D_ASSERT(prepared_statements);
  //   db_.transaction_manager->AddCatalogSet(*this, move(prepared_statements_));
  //   // invalidate any prepared statements
  //   for (auto &statement : prepared_statement_objects) {
  //     statement->is_invalidated = true;
  //   }
  //   for (auto &appender : appenders) {
  //     appender->Invalidate("Connection has been closed!", false);
  //   }
  //   CleanupInternal();
}

// void ClientContext::RegisterAppender(Appender *appender) {
//   // lock_guard<mutex> client_guard(context_lock);
//   // if (is_invalidated) {
//   //   throw Exception("Database that this connection belongs to has been closed!");
//   // }
//   // appenders.insert(appender);
// }

// void ClientContext::RemoveAppender(Appender *appender) {
//   // lock_guard<mutex> client_guard(context_lock);
//   // if (is_invalidated) {
//   //   return;
//   // }
//   // appenders.erase(appender);
// }

unique_ptr<Tuple> ClientContext::Fetch() {
  // lock_guard<mutex> client_guard(context_lock_);
  // if (is_invalidated_) {
  //   // ClientContext is invalidated: database has been closed
  //   open_result->error = "Database that this connection belongs to has been closed!";
  //   open_result->success = false;
  //   return nullptr;
  // }

  return FetchInternal();
}

string ClientContext::FinalizeQuery(bool success) {
  LOG_INFO("FINALIZING QUERY. success: %d.", success);
  // executor_.Reset();

  string error;
  // if (transaction.HasActiveTransaction()) {
  //   ActiveTransaction().active_query = MAXIMUM_QUERY_ID;
  //   try {
  //     if (transaction.IsAutoCommit()) {
  //       if (success) {
  //         // query was successful: commit
  //         transaction.Commit();
  //       } else {
  //         // query was unsuccessful: rollback
  //         transaction.Rollback();
  //       }
  //     }
  //   } catch (Exception &ex) {
  //     error = ex.what();
  //   } catch (...) {
  //     error = "Unhandled exception!";
  //   }
  // }
  return error;
}

void ClientContext::CleanupInternal() {
  // if (!open_result) {
  //   // no result currently open
  //   return;
  // }

  // auto error = FinalizeQuery(open_result->success);
  // if (open_result->success) {
  //   // if an error occurred while committing report it in the result
  //   open_result->error = error;
  //   open_result->success = error.empty();
  // }

  // open_result->is_open = false;
  // open_result = nullptr;

  // this->query_ = string();
}

unique_ptr<Tuple> ClientContext::FetchInternal() {
  // constructor for creating a new tuple based on input value
  static int counter = 0;
  counter++;

  if ((counter % 100) == 0) {
    return nullptr;
  }

  std::vector<Column> columns;
  auto column1 = Column("column1", TypeId::INTEGER, nullptr);
  columns.push_back(column1);
  Schema schema = Schema(columns);

  std::vector<Value> values;
  values.emplace_back(Value(TypeId::INTEGER, counter));
  return make_unique<Tuple>(values, &schema);
  // return executor_.FetchChunk();
}

unique_ptr<PreparedStatementData> ClientContext::CreatePreparedStatement(const string &query,
                                                                         unique_ptr<SQLStatement> statement) {
  StatementType statement_type = statement->type_;
  auto result = make_unique<PreparedStatementData>(statement_type);

  // Planner planner(*this);
  // planner.CreatePlan(move(statement));
  // D_ASSERT(planner.plan);

  // auto plan = move(planner.plan);
  // extract the result column names from the plan
  // result->read_only = planner.read_only;
  // result->requires_valid_transaction = planner.requires_valid_transaction;

  result->names_ = {"column1"};
  result->types_ = {Type(TypeId::INTEGER)};
  // result->value_map_ = move(planner.value_map);

  // if (enable_optimizer) {
  //   profiler.StartPhase("optimizer");
  //   Optimizer optimizer(planner.binder, *this);
  //   plan = optimizer.Optimize(move(plan));
  //   D_ASSERT(plan);
  //   profiler.EndPhase();
  // }

  // profiler.StartPhase("physical_planner");
  // // now convert logical query plan into a physical query plan
  // PhysicalPlanGenerator physical_planner(*this);
  // auto physical_plan = physical_planner.CreatePlan(move(plan));
  // profiler.EndPhase();

  // result->dependencies = move(physical_planner.dependencies);
  // result->plan = move(physical_plan);

  return result;
}

unique_ptr<QueryResult> ClientContext::ExecutePreparedStatement(const string &query, PreparedStatementData &statement,
                                                                vector<Value> bound_values, bool allow_stream_result) {
  // if (ActiveTransaction().is_invalidated && statement.requires_valid_transaction) {
  //   throw Exception("Current transaction is aborted (please ROLLBACK)");
  // }
  // if (db_->config_.access_mode == AccessMode::READ_ONLY && !statement.read_only) {
  //   throw Exception(StringUtil::Format("Cannot execute statement of type \"%s\" in read-only mode!",
  //                                      StatementTypeToString(statement.statement_type)));
  // }

  // // bind the bound values before execution
  // statement.Bind(move(bound_values));

  // // store the physical plan in the context for calls to Fetch()
  // executor_.Initialize(move(statement.plan_));

  // auto types = executor_.GetTypes();

  // create a materialized result by continuously fetching
  // auto result = make_unique<QueryResult>(statement.statement_type, statement.types, statement.names);

  auto result = make_unique<QueryResult>();

  while (true) {
    auto tuple = FetchInternal();
    if (tuple == nullptr) {
      break;
    }

    result->data_.push_back(move(tuple));
  }

  result->success_ = true;

  return result;
}

void ClientContext::InitialCleanup() {
  // if (is_invalidated) {
  //   throw Exception("Database that this connection belongs to has been closed!");
  // }
  // //! Cleanup any open results and reset the interrupted flag
  // CleanupInternal();
  // interrupted = false;
}

vector<unique_ptr<SQLStatement>> ClientContext::ParseStatements(string query) {
  Parser parser;
  parser.ParseQuery(query);

  // PragmaHandler handler(*this);
  // handler.HandlePragmaStatements(parser.statements);

  return move(parser.statements_);
}

unique_ptr<PreparedStatement> ClientContext::PrepareInternal(unique_ptr<SQLStatement> statement) {
  auto n_param = statement->n_param_;
  static int prepare_count = 0;
  // now write the prepared statement data into the catalog
  string prepare_name = "____bustub_internal_prepare_" + std::to_string(prepare_count);
  prepare_count_++;

  auto statement_query = statement->query_;
  auto unbound_statement = statement->Copy();

  // now perform the actual PREPARE query
  auto result = RunStatement(statement->query_, move(statement), false);

  if (!result->success_) {
    LOG_DEBUG("prepare internal failed");
    throw Exception(result->error_);
  }


  LOG_DEBUG("about to copy unbound statement");
  shared_ptr<PreparedStatementData> prepared_data = make_shared<PreparedStatementData>(StatementType::SELECT_STATEMENT);
  prepared_data->unbound_statement_ = move(unbound_statement);
  LOG_DEBUG("done copying to unbound statement");
  // PreparedStatement(shared_ptr<ClientContext> context, shared_ptr<PreparedStatementData> data, string query,
  //                   int64_t n_param);

  LOG_INFO("stmt query: %s", statement_query.c_str());

  auto t1 = shared_from_this();

  auto result_stmt = make_unique<PreparedStatement>(t1, move(prepared_data), move(statement_query), n_param);

  //! Create a successfully prepared prepared statement object with the given name
  return result_stmt;
}

unique_ptr<PreparedStatement> ClientContext::Prepare(unique_ptr<SQLStatement> statement) {
  // lock_guard<mutex> client_guard(context_lock_);
  // prepare the query
  try {
    // InitialCleanup();
    return PrepareInternal(move(statement));
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return std::make_unique<PreparedStatement>(ex.what());
  }
}

unique_ptr<PreparedStatement> ClientContext::Prepare(string query) {
  // lock_guard<mutex> client_guard(context_lock);
  // prepare the query
  try {
    // InitialCleanup();
    throw Exception("ASDFASDFSDF!");

    // // first parse the query
    // auto statements = ParseStatements(query);
    // if (statements.size() == 0) {
    //   throw Exception("No statement to prepare!");
    // }
    // if (statements.size() > 1) {
    //   throw Exception("Cannot prepare multiple statements at once!");
    // }
    // return PrepareInternal(move(statements[0]));
  } catch (Exception &ex) {
    LOG_DEBUG("PREPARE FAILED");
    return make_unique<PreparedStatement>(ex.what());
  }
}

unique_ptr<QueryResult> ClientContext::Execute(string name, vector<Value> &values, bool allow_stream_result,
                                               string query) {
  // lock_guard<mutex> client_guard(context_lock);
  try {
    InitialCleanup();
  } catch (std::exception &ex) {
    // return make_unique<QueryResult>(ex.what());
    return make_unique<QueryResult>();
  }

  // // create the execute statement
  // auto execute = make_unique<ExecuteStatement>();
  // execute->name = name;
  // for (auto &val : values) {
  //   execute->values.push_back(make_unique<ConstantExpression>(val));
  // }

  throw NotImplementedException("jkldfdsflk;");

  // return RunStatement(query, move(execute), allow_stream_result);
  return nullptr;
}

void ClientContext::RemovePreparedStatement(PreparedStatement *statement) {
  // // lock_guard<mutex> client_guard(context_lock);
  // if (!statement->success_ || statement->is_invalidated || is_invalidated) {
  //   return;
  // }
  // try {
  //   InitialCleanup();
  // } catch (...) {
  //   return;
  // }
  // // erase the object from the list of prepared statements
  // prepared_statement_objects_.erase(statement);
  // // drop it from the catalog
  // auto deallocate_statement = make_unique<DropStatement>();
  // deallocate_statement->info->type = CatalogType::PREPARED_STATEMENT;
  // deallocate_statement->info->name = statement->name;
  // string query = "DEALLOCATE " + statement->name;
  // RunStatement(query, move(deallocate_statement), false);
}

unique_ptr<QueryResult> ClientContext::RunStatementInternal(const string &query, unique_ptr<SQLStatement> statement,
                                                            bool allow_stream_result) {
  // prepare the query for execution
  auto prepared = CreatePreparedStatement(query, move(statement));
  // by default, no values are bound
  vector<Value> bound_values;
  // execute the prepared statement
  return ExecutePreparedStatement(query, *prepared, move(bound_values), allow_stream_result);
}

unique_ptr<QueryResult> ClientContext::RunStatement(const string &query, unique_ptr<SQLStatement> statement,
                                                    bool allow_stream_result) {
  // this->query_ = query;

  unique_ptr<QueryResult> result;
  // // check if we are on AutoCommit. In this case we should start a transaction.
  // if (transaction_.IsAutoCommit()) {
  //   transaction_.BeginTransaction();
  // }
  // ActiveTransaction().active_query = db_.transaction_manager_->GetQueryNumber();

  try {
    result = RunStatementInternal(query, move(statement), allow_stream_result);
  } catch (std::exception &ex) {
    // other types of exceptions do invalidate the current transaction
    // if (transaction_.HasActiveTransaction()) {
    //   ActiveTransaction().is_invalidated = true;
    // }

    LOG_DEBUG("RUNSTATEMENT INTERNAL THREW EXCEPTION");
    result = make_unique<QueryResult>(ex.what());
  }

  if (!result->success_) {
    // query failed: abort now
    FinalizeQuery(false);
    return result;
  }

  string error = FinalizeQuery(true);
  if (!error.empty()) {
    // failure in committing transaction
    return make_unique<QueryResult>(error);
  }

  return result;
}

unique_ptr<QueryResult> ClientContext::RunStatements(const string &query, vector<unique_ptr<SQLStatement>> &statements,
                                                     bool allow_stream_result) {
  // now we have a list of statements
  // iterate over them and execute them one by one
  unique_ptr<QueryResult> result;
  // QueryResult *last_result = nullptr;
  for (uint64_t i = 0; i < statements.size(); i++) {
    auto &statement = statements[i];
    bool is_last_statement = i + 1 == statements.size();
    auto current_result = RunStatement(query, move(statement), allow_stream_result && is_last_statement);
    // // now append the result to the list of results
    // if (!last_result) {
    //   // first result of the query
    //   result = move(current_result);
    //   last_result = result.get();
    // } else {
    //   // later results; attach to the result chain
    //   last_result->next = move(current_result);
    //   last_result = last_result->next.get();
    // }
  }
  return result;
}

void ClientContext::LogQueryInternal(string query) {
  // if (!log_query_writer) {
  //   return;
  // }
  // // log query path is set: log the query
  // log_query_writer->WriteData((const_data_ptr_t)query.c_str(), query.size());
  // log_query_writer->WriteData((const_data_ptr_t) "\n", 1);
  // log_query_writer->Flush();
}

unique_ptr<QueryResult> ClientContext::Query(unique_ptr<SQLStatement> statement, bool allow_stream_result) {
  // lock_guard<mutex> client_guard(context_lock);
  // if (log_query_writer) {
  //   LogQueryInternal(statement->query.substr(statement->stmt_location, statement->stmt_length));
  // }

  vector<unique_ptr<SQLStatement>> statements;
  statements.push_back(move(statement));

  return RunStatements(query_, statements, allow_stream_result);
}

unique_ptr<QueryResult> ClientContext::Query(string query, bool allow_stream_result) {
  // lock_guard<mutex> client_guard(context_lock);
  // LogQueryInternal(query);

  // vector<unique_ptr<SQLStatement>> statements;
  // try {
  //   InitialCleanup();
  //   // parse the query and transform it into a set of statements
  //   statements = ParseStatements(query);
  // } catch (std::exception &ex) {
  //   return std::make_unique<QueryResult>(ex.what());
  // }

  // if (statements.size() == 0) {
  //   // no statements, return empty successful result
  //   return std::make_unique<QueryResult>(StatementType::INVALID_STATEMENT);
  // }

  // return RunStatements(query, statements, allow_stream_result);
  LOG_DEBUG("QUERY FAILED");
  throw NotImplementedException("QUERY FAILED");

  return nullptr;
}

void ClientContext::Interrupt() {
  // interrupted_ = true;
}

// void ClientContext::EnableProfiling() {
//   lock_guard<mutex> client_guard(context_lock);
//   profiler.Enable();
// }

// void ClientContext::DisableProfiling() {
//   lock_guard<mutex> client_guard(context_lock);
//   profiler.Disable();
// }

void ClientContext::Invalidate() {
  // // interrupt any running query before attempting to obtain the lock
  // // this way we don't have to wait for the entire query to finish
  // Interrupt();
  // // now obtain the context lock
  // lock_guard<mutex> client_guard(context_lock);
  // // invalidate this context and the TransactionManager
  // is_invalidated = true;
  // transaction.Invalidate();
  // // also close any open result
  // if (open_result) {
  //   open_result->is_open = false;
  // }
  // // and close any open appenders and prepared statements
  // for (auto &statement : prepared_statement_objects) {
  //   statement->is_invalidated = true;
  // }
  // for (auto &appender : appenders) {
  //   appender->Invalidate("Database that this appender belongs to has been closed!", false);
  // }
  // appenders.clear();
  // // clear temporary objects and prepared statemnts
  // temporary_objects.reset();
  // prepared_statements.reset();
}

string ClientContext::VerifyQuery(string query, unique_ptr<SQLStatement> statement) {
  // D_ASSERT(statement->type == StatementType::SELECT_STATEMENT);
  // aggressive query verification

  // the purpose of this function is to test correctness of otherwise hard to test features:
  // Copy() of statements and expressions
  // Serialize()/Deserialize() of expressions
  // Hash() of expressions
  // Equality() of statements and expressions
  // Correctness of plans both with and without optimizers
  // Correctness of plans both with and without parallelism

  //   // copy the statement
  //   auto select_stmt = (SelectStatement *)statement.get();
  //   auto copied_stmt = select_stmt->Copy();
  //   auto unoptimized_stmt = select_stmt->Copy();

  //   BufferedSerializer serializer;
  //   select_stmt->Serialize(serializer);
  //   BufferedDeserializer source(serializer);
  //   auto deserialized_stmt = SelectStatement::Deserialize(source);
  //   // all the statements should be equal
  //   D_ASSERT(copied_stmt->Equals(statement.get()));
  //   D_ASSERT(deserialized_stmt->Equals(statement.get()));
  //   D_ASSERT(copied_stmt->Equals(deserialized_stmt.get()));

  //   // now perform checking on the expressions
  // #ifdef DEBUG
  //   auto &orig_expr_list = select_stmt->node->GetSelectList();
  //   auto &de_expr_list = deserialized_stmt->node->GetSelectList();
  //   auto &cp_expr_list = copied_stmt->node->GetSelectList();
  //   D_ASSERT(orig_expr_list.size() == de_expr_list.size() && cp_expr_list.size() == de_expr_list.size());
  //   for (idx_t i = 0; i < orig_expr_list.size(); i++) {
  //     // run the ToString, to verify that it doesn't crash
  //     orig_expr_list[i]->ToString();
  //     // check that the expressions are equivalent
  //     D_ASSERT(orig_expr_list[i]->Equals(de_expr_list[i].get()));
  //     D_ASSERT(orig_expr_list[i]->Equals(cp_expr_list[i].get()));
  //     D_ASSERT(de_expr_list[i]->Equals(cp_expr_list[i].get()));
  //     // check that the hashes are equivalent too
  //     D_ASSERT(orig_expr_list[i]->Hash() == de_expr_list[i]->Hash());
  //     D_ASSERT(orig_expr_list[i]->Hash() == cp_expr_list[i]->Hash());
  //   }
  //   // now perform additional checking within the expressions
  //   for (idx_t outer_idx = 0; outer_idx < orig_expr_list.size(); outer_idx++) {
  //     auto hash = orig_expr_list[outer_idx]->Hash();
  //     for (idx_t inner_idx = 0; inner_idx < orig_expr_list.size(); inner_idx++) {
  //       auto hash2 = orig_expr_list[inner_idx]->Hash();
  //       if (hash != hash2) {
  //         // if the hashes are not equivalent, the expressions should not be equivalent
  //         D_ASSERT(!orig_expr_list[outer_idx]->Equals(orig_expr_list[inner_idx].get()));
  //       }
  //     }
  //   }
  // #endif

  //   // disable profiling if it is enabled
  //   bool profiling_is_enabled = profiler.IsEnabled();
  //   if (profiling_is_enabled) {
  //     profiler.Disable();
  //   }

  //   // see below
  //   auto statement_copy_for_explain = select_stmt->Copy();

  //   unique_ptr<MaterializedQueryResult> original_result =
  //                                           make_unique<MaterializedQueryResult>(StatementType::SELECT_STATEMENT),
  //                                       copied_result =
  //                                           make_unique<MaterializedQueryResult>(StatementType::SELECT_STATEMENT),
  //                                       deserialized_result =
  //                                           make_unique<MaterializedQueryResult>(StatementType::SELECT_STATEMENT),
  //                                       unoptimized_result =
  //                                           make_unique<MaterializedQueryResult>(StatementType::SELECT_STATEMENT);

  //   // execute the original statement
  //   try {
  //     auto result = RunStatementInternal(query, move(statement), false);
  //     original_result = unique_ptr_cast<QueryResult, MaterializedQueryResult>(move(result));
  //   } catch (std::exception &ex) {
  //     original_result->error = ex.what();
  //     original_result->success = false;
  //     interrupted = false;
  //   }

  //   // check explain, only if q does not already contain EXPLAIN
  //   if (original_result->success) {
  //     auto explain_q = "EXPLAIN " + query;
  //     auto explain_stmt = make_unique<ExplainStatement>(move(statement_copy_for_explain));
  //     try {
  //       RunStatementInternal(explain_q, move(explain_stmt), false);
  //     } catch (std::exception &ex) {
  //       return "EXPLAIN failed but query did not (" + string(ex.what()) + ")";
  //     }
  //   }

  //   // now execute the copied statement
  //   try {
  //     auto result = RunStatementInternal(query, move(copied_stmt), false);
  //     copied_result = unique_ptr_cast<QueryResult, MaterializedQueryResult>(move(result));
  //   } catch (std::exception &ex) {
  //     copied_result->error = ex.what();
  //     copied_result->success = false;
  //     interrupted = false;
  //   }
  //   // now execute the deserialized statement
  //   try {
  //     auto result = RunStatementInternal(query, move(deserialized_stmt), false);
  //     deserialized_result = unique_ptr_cast<QueryResult, MaterializedQueryResult>(move(result));
  //   } catch (std::exception &ex) {
  //     deserialized_result->error = ex.what();
  //     deserialized_result->success = false;
  //     interrupted = false;
  //   }
  //   // now execute the unoptimized statement
  //   enable_optimizer = false;
  //   try {
  //     auto result = RunStatementInternal(query, move(unoptimized_stmt), false);
  //     unoptimized_result = unique_ptr_cast<QueryResult, MaterializedQueryResult>(move(result));
  //   } catch (std::exception &ex) {
  //     unoptimized_result->error = ex.what();
  //     unoptimized_result->success = false;
  //     interrupted = false;
  //   }
  //   enable_optimizer = true;

  //   if (profiling_is_enabled) {
  //     profiler.Enable();
  //   }

  //   // now compare the results
  //   // the results of all runs should be identical
  //   vector<unique_ptr<MaterializedQueryResult>> results;
  //   results.push_back(move(copied_result));
  //   results.push_back(move(deserialized_result));
  //   results.push_back(move(unoptimized_result));
  //   vector<string> names = {"Copied Result", "Deserialized Result", "Unoptimized Result"};
  //   for (idx_t i = 0; i < results.size(); i++) {
  //     if (original_result->success != results[i]->success) {
  //       string result = names[i] + " differs from original result!\n";
  //       result += "Original Result:\n" + original_result->ToString();
  //       result += names[i] + ":\n" + results[i]->ToString();
  //       return result;
  //     }
  //     if (!original_result->collection.Equals(results[i]->collection)) {
  //       string result = names[i] + " differs from original result!\n";
  //       result += "Original Result:\n" + original_result->ToString();
  //       result += names[i] + ":\n" + results[i]->ToString();
  //       return result;
  //     }
  //   }

  return "";
}

// void ClientContext::RegisterFunction(CreateFunctionInfo *info) {
//   RunFunctionInTransaction([&]() { temporary_objects.get()->CreateFunction(*this, info); });
// }

// void ClientContext::RunFunctionInTransactionInternal(std::function<void(void)> fun) {
//   if (transaction.HasActiveTransaction() && transaction.ActiveTransaction().is_invalidated) {
//     throw Exception("Failed: transaction has been invalidated!");
//   }
//   // check if we are on AutoCommit. In this case we should start a transaction
//   if (transaction.IsAutoCommit()) {
//     transaction.BeginTransaction();
//   }
//   try {
//     fun();
//   } catch (Exception &ex) {
//     if (transaction.IsAutoCommit()) {
//       transaction.Rollback();
//     } else {
//       transaction.Invalidate();
//     }
//     throw ex;
//   }
//   if (transaction.IsAutoCommit()) {
//     transaction.Commit();
//   }
// }

// void ClientContext::RunFunctionInTransaction(std::function<void(void)> fun) {
//   lock_guard<mutex> client_guard(context_lock);
//   if (is_invalidated) {
//     throw Exception("Failed: database has been closed!");
//   }
//   RunFunctionInTransactionInternal(fun);
// }

// unique_ptr<TableDescription> ClientContext::TableInfo(string schema_name, string table_name) {
//   unique_ptr<TableDescription> result;
//   // RunFunctionInTransaction([&]() {
//   //   // obtain the table info
//   //   auto table = db.catalog->GetEntry<TableCatalogEntry>(*this, schema_name, table_name, true);
//   //   if (!table) {
//   //     return;
//   //   }
//   //   // write the table info to the result
//   //   result = make_unique<TableDescription>();
//   //   result->schema = schema_name;
//   //   result->table = table_name;
//   //   for (auto &column : table->columns) {
//   //     result->columns.push_back(ColumnDefinition(column.name, column.type));
//   //   }
//   // });
//   return result;
// }

// void ClientContext::Append(TableDescription &description, DataChunk &chunk) {
//   // RunFunctionInTransaction([&]() {
//   //   auto table_entry = db.catalog->GetEntry<TableCatalogEntry>(*this, description.schema, description.table);
//   //   // verify that the table columns and types match up
//   //   if (description.columns.size() != table_entry->columns.size()) {
//   //     throw Exception("Failed to append: table entry has different number of columns!");
//   //   }
//   //   for (idx_t i = 0; i < description.columns.size(); i++) {
//   //     if (description.columns[i].type != table_entry->columns[i].type) {
//   //       throw Exception("Failed to append: table entry has different number of columns!");
//   //     }
//   //   }
//   //   table_entry->storage->Append(*table_entry, *this, chunk);
//   // });
// }

// void ClientContext::TryBindRelation(Relation &relation, vector<ColumnDefinition> &result_columns) {
//   RunFunctionInTransaction([&]() {
//     // bind the expressions
//     Binder binder(*this);
//     auto result = relation.Bind(binder);
//     D_ASSERT(result.names.size() == result.types.size());
//     for (idx_t i = 0; i < result.names.size(); i++) {
//       result_columns.push_back(ColumnDefinition(result.names[i], result.types[i]));
//     }
//   });
// }

// unique_ptr<QueryResult> ClientContext::Execute(shared_ptr<Relation> relation) {
//   string query;
//   // if (query_verification_enabled) {
//   //   // run the ToString method of any relation we run, mostly to ensure it doesn't crash
//   //   relation->ToString();
//   //   if (relation->IsReadOnly()) {
//   //     // verify read only statements by running a select statement
//   //     auto select = make_unique<SelectStatement>();
//   //     select->node = relation->GetQueryNode();
//   //     RunStatement(query, move(select), false);
//   //   }
//   // }
//   // auto &expected_columns = relation->Columns();
//   // auto relation_stmt = std::make_unique<RelationStatement>(relation);
//   // auto result = RunStatement(query, move(relation_stmt), false);
//   // if (!result->success) {
//   //   return result;
//   // }
//   // // verify that the result types and result names of the query match the expected result types/names
//   // if (result->types.size() == expected_columns.size()) {
//   //   bool mismatch = false;
//   //   for (int64_t i = 0; i < result->types.size(); i++) {
//   //     if (result->types[i] != expected_columns[i].type || result->names[i] != expected_columns[i].name) {
//   //       mismatch = true;
//   //       break;
//   //     }
//   //   }
//   //   if (!mismatch) {
//   //     // all is as expected: return the result
//   //     return result;
//   //   }
//   // }
//   // // result mismatch
//   // string err_str = "Result mismatch in query!\nExpected the following columns: ";
//   // for (int64_t i = 0; i < expected_columns.size(); i++) {
//   //   err_str += i == 0 ? "[" : ", ";
//   //   err_str += expected_columns[i].name + " " + expected_columns[i].type.ToString();
//   // }
//   // err_str += "]\nBut result contained the following: ";
//   // for (int64_t i = 0; i < result->types.size(); i++) {
//   //   err_str += i == 0 ? "[" : ", ";
//   //   err_str += result->names[i] + " " + result->types[i].ToString();
//   // }
//   // err_str += "]";
//   // return std::make_unique<QueryResult>(err_str);
//   return std::make_unique<QueryResult>();
// }

}  // namespace bustub
