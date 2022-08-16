//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// execution_engine.h
//
// Identification: src/include/execution/execution_engine.h
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>

#include "buffer/buffer_pool_manager.h"
#include "catalog/catalog.h"
#include "concurrency/transaction_manager.h"
#include "execution/executor_factory.h"
#include "main/client_context.h"
#include "main/database.h"
#include "planner/plans/abstract_plan.h"
#include "storage/table/tuple.h"

namespace bustub {
class ClientContext;

/**
 * The ExecutionEngine class executes query plans.
 */
class ExecutionEngine {
 public:
  /**
   * Construct a new ExecutionEngine instance.
   * @param bpm The buffer pool manager used by the execution engine
   * @param txn_mgr The transaction manager used by the execution engine
   * @param catalog The catalog used by the execution engine
   */
  explicit ExecutionEngine(ClientContext &context);

  DISALLOW_COPY_AND_MOVE(ExecutionEngine);

  /**
   * Execute a query plan.
   * @param plan The query plan to execute
   * @param result_set The set of tuples produced by executing the plan
   * @param txn The transaction context in which the query executes
   * @return `true` if execution of the query plan succeeds, `false` otherwise
   */
  auto Execute(AbstractPlanNode *plan, std::vector<Tuple> *result_set, Transaction *txn) -> bool {
    // Construct and executor for the plan
    // Prepare the root executor
    auto executor = ExecutorFactory::CreateExecutor(context_, plan);

    // Prepare the root executor
    executor->Init();

    // Execute the query plan
    try {
      Tuple tuple;
      RID rid;
      while (executor->Next(&tuple, &rid)) {
        if (result_set != nullptr) {
          result_set->push_back(tuple);
        }
      }
    } catch (Exception &e) {
      // TODO(student): handle exceptions
    }

    return true;
  }

  ExecutionEngine &Get(DatabaseInstance &db);

 private:
  [[maybe_unused]] ClientContext &context_;
};

}  // namespace bustub
