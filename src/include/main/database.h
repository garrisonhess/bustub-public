//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// database.h
//
// Identification: src/include/main/database.h
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.h"
#include "main/config.h"

namespace bustub {
class CheckpointManager;
class DiskManager;
class LogManager;
class LockManager;
class BufferPoolManagerInstance;
class TransactionManager;
class Catalog;

class DatabaseInstance : public std::enable_shared_from_this<DatabaseInstance> {
  friend class BusTub;

 public:
  DatabaseInstance() = default;
  ~DatabaseInstance() = default;

  DBConfig config_;

 public:
  Catalog &GetCatalog();
  TransactionManager &GetTransactionManager();
  BufferPoolManagerInstance &GetBufferPoolManager();
  LogManager &GetLogManager();
  DiskManager &GetDiskManager();
  CheckpointManager &GetCheckpointManager();
  LockManager &GetLockManager();

  // int64_t NumberOfThreads();
  // static DatabaseInstance &GetDatabase(ClientContext &context);

 private:
  void Initialize(const char *path, DBConfig *config);

 private:
  unique_ptr<BufferPoolManagerInstance> buffer_pool_manager_;
  unique_ptr<Catalog> catalog_;
  unique_ptr<CheckpointManager> checkpoint_manager_;
  unique_ptr<DiskManager> disk_manager_;
  unique_ptr<LogManager> log_manager_;
  unique_ptr<LockManager> lock_manager_;
  unique_ptr<TransactionManager> transaction_manager_;
};

//! The database object. This object holds the catalog and all the
//! database-specific meta information.
class BusTub {
 public:
  explicit BusTub(const char *path = nullptr, DBConfig *config = nullptr);
  explicit BusTub(const string &path, DBConfig *config = nullptr);
  explicit BusTub(DatabaseInstance &instance);

  ~BusTub() = default;

  shared_ptr<DatabaseInstance> instance_;
};

}  // namespace bustub
