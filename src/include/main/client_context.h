// //===----------------------------------------------------------------------===//
// //                         DuckDB
// //
// // main/client_context.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "main/database.h"

// #include "execution/execution_engine.h"

// // #include "catalog/catalog_entry/schema_catalog_entry.h"
// // #include "catalog/catalog_set.h"
// // #include "execution/executor.h"
// // #include "main/prepared_statement.h"
// // #include "main/stream_query_result.h"
// // #include "main/table_description.h"
// // #include "transaction/transaction_context.h"
// // #include "common/enums/output_type.h"

// #include <unordered_set>

// #include <memory>
// #include <mutex>
// #include <random>

// namespace bustub {
// using std::string;
// using std::unique_ptr;

// class Appender;
// class Catalog;
// class BusTub;
// class PreparedStatementData;
// class Relation;
// class BufferedFileWriter;

// //! The ClientContext holds information relevant to the current client session
// //! during execution
// class ClientContext {
//  public:
//   explicit ClientContext(BusTub &db);

//   //! The database that this client is connected to
//   BusTub &db_;

//   //! The current query being executed by the client context
//   string query_;

//   //! The query executor (has transaction manager and catalog)
//   ExecutionEngine *executor_;

//  public:
//   Transaction &ActiveTransaction() { return transaction_.ActiveTransaction(); }

//   //! Interrupt execution of a query
//   void Interrupt();

//   //! Issue a query, returning a QueryResult. The QueryResult can be either a StreamQueryResult or a
//   //! MaterializedQueryResult. The StreamQueryResult will only be returned in the case of a successful SELECT
//   //! statement.
//   unique_ptr<QueryResult> Query(string query, bool allow_stream_result);
//   unique_ptr<QueryResult> Query(unique_ptr<SQLStatement> statement, bool allow_stream_result);

//   //! Fetch a query from the current result set (if any)
//   unique_ptr<DataChunk> Fetch();

//   //! Cleanup the result set (if any).
//   void Cleanup();

//   //! Invalidate the client context. The current query will be interrupted and the client context will be
//   // invalidated, making it impossible for future queries to run.
//   void Invalidate();

//   //! Get the table info of a specific table, or nullptr if it cannot be found
//   unique_ptr<TableDescription> TableInfo(string schema_name, string table_name);
//   //! Appends a DataChunk to the specified table. Returns whether or not the append was successful.
//   void Append(TableDescription &description, DataChunk &chunk);
//   //! Try to bind a relation in the current client context; either throws an exception or fills the result_columns
//   //! list with the set of returned columns
//   void TryBindRelation(Relation &relation, vector<ColumnDefinition> &result_columns);

//   //! Execute a relation
//   unique_ptr<QueryResult> Execute(shared_ptr<Relation> relation);

//   //! Prepare a query
//   unique_ptr<PreparedStatement> Prepare(string query);
//   //! Directly prepare a SQL statement
//   unique_ptr<PreparedStatement> Prepare(unique_ptr<SQLStatement> statement);

//   //! Execute a prepared statement with the given name and set of parameters
//   unique_ptr<QueryResult> Execute(string name, vector<Value> &values, bool allow_stream_result = true,
//                                   string query = string());
//   //! Removes a prepared statement from the set of prepared statements in the client context
//   void RemovePreparedStatement(PreparedStatement *statement);

//   void RegisterAppender(Appender *appender);
//   void RemoveAppender(Appender *appender);

//   //! Register function in the temporary schema
//   void RegisterFunction(CreateFunctionInfo *info);

//   //! Parse statements from a query
//   vector<unique_ptr<SQLStatement>> ParseStatements(string query);

//   //! Runs a function with a valid transaction context, potentially starting a transaction if the context is in
//   auto
//       //! commit mode.
//       void
//       RunFunctionInTransaction(std::function<void(void)> fun);
//   //! Same as RunFunctionInTransaction, but does not obtain a lock on the client context or check for validation
//   void RunFunctionInTransactionInternal(std::function<void(void)> fun);

//  private:
//   //! Perform aggressive query verification of a SELECT statement. Only called when query_verification_enabled is
//   //! true.
//   string VerifyQuery(string query, unique_ptr<SQLStatement> statement);

//   void InitialCleanup();
//   //! Internal clean up, does not lock. Caller must hold the context_lock.
//   void CleanupInternal();
//   string FinalizeQuery(bool success);
//   //! Internal fetch, does not lock. Caller must hold the context_lock.
//   unique_ptr<DataChunk> FetchInternal();
//   //! Internally execute a set of SQL statement. Caller must hold the context_lock.
//   unique_ptr<QueryResult> RunStatements(const string &query, vector<unique_ptr<SQLStatement>> &statements,
//                                         bool allow_stream_result);
//   //! Internally prepare and execute a prepared SQL statement. Caller must hold the context_lock.
//   unique_ptr<QueryResult> RunStatement(const string &query, unique_ptr<SQLStatement> statement,
//                                        bool allow_stream_result);

//   //! Internally prepare a SQL statement. Caller must hold the context_lock.
//   unique_ptr<PreparedStatementData> CreatePreparedStatement(const string &query, unique_ptr<SQLStatement> statement);
//   //! Internally execute a prepared SQL statement. Caller must hold the context_lock.
//   unique_ptr<QueryResult> ExecutePreparedStatement(const string &query, PreparedStatementData &statement,
//                                                    vector<Value> bound_values, bool allow_stream_result);
//   //! Call CreatePreparedStatement() and ExecutePreparedStatement() without any bound values
//   unique_ptr<QueryResult> RunStatementInternal(const string &query, unique_ptr<SQLStatement> statement,
//                                                bool allow_stream_result);
//   unique_ptr<PreparedStatement> PrepareInternal(unique_ptr<SQLStatement> statement);
//   void LogQueryInternal(string query);

//  private:
//   idx_t prepare_count = 0;
//   //! The currently opened StreamQueryResult (if any)
//   StreamQueryResult *open_result = nullptr;
//   //! Prepared statement objects that were created using the ClientContext::Prepare method
//   unordered_set<PreparedStatement *> prepared_statement_objects;
//   //! Appenders that were attached to this client context
//   unordered_set<Appender *> appenders;
// };
// }  // namespace bustub
