//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// transaction_manager.h
//
// Identification: src/include/concurrency/transaction_manager.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <atomic>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

#include "concurrency/lock_manager.h"
#include "concurrency/transaction.h"
#include "main/config.h"
#include "main/database.h"
#include "recovery/log_manager.h"

namespace bustub {
class LockManager;
class DatabaseInstance;

/**
 * TransactionManager keeps track of all the transactions running in the system.
 */
class TransactionManager {
 public:
  explicit TransactionManager(DatabaseInstance &db);

  ~TransactionManager() = default;

  /**
   * Begins a new transaction.
   * @param txn an optional transaction object to be initialized, otherwise a new transaction is created.
   * @param isolation_level an optional isolation level of the transaction.
   * @return an initialized transaction
   */
  Transaction *Begin(Transaction *txn = nullptr, IsolationLevel isolation_level = IsolationLevel::REPEATABLE_READ);

  /**
   * Commits a transaction.
   * @param txn the transaction to commit
   */
  void Commit(Transaction *txn);

  /**
   * Aborts a transaction
   * @param txn the transaction to abort
   */
  void Abort(Transaction *txn);

  /**
   * Locates and returns the transaction with the given transaction ID.
   * @param txn_id the id of the transaction to be found, it must exist!
   * @return the transaction with the given transaction id
   */
  static Transaction *GetTransaction(txn_id_t txn_id);

  /** Prevents all transactions from performing operations, used for checkpointing. */
  void BlockAllTransactions();

  /** Resumes all transactions, used for checkpointing. */
  void ResumeTransactions();

  /**
   * Releases all the locks held by the given transaction.
   * @param txn the transaction whose locks should be released
   */
  void ReleaseLocks(Transaction *txn);

  static TransactionManager &Get(DatabaseInstance &db);

  /**
   * Global list of running transactions
   */
  /** The transaction map is a global list of all the running transactions in the system. */
  static std::unordered_map<txn_id_t, Transaction *> txn_map;
  static std::shared_mutex txn_map_mutex;

 private:
  std::atomic<txn_id_t> next_txn_id_{0};

  //! The database instance
  DatabaseInstance &db_;

  /** The global transaction latch is used for checkpointing. */
  ReaderWriterLatch global_txn_latch_;
};

}  // namespace bustub
