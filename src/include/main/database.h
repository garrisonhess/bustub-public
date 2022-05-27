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

#include "buffer/buffer_pool_manager_instance.h"
#include "common/constants.h"
#include "concurrency/lock_manager.h"
#include "main/config.h"
#include "recovery/checkpoint_manager.h"
#include "recovery/log_manager.h"
#include "storage/disk/disk_manager.h"

namespace bustub {

class DatabaseInstance {
  friend class BusTub;

 public:
  DatabaseInstance();
  ~DatabaseInstance();

  DBConfig config_;

 public:
  Catalog &GetCatalog();
  TransactionManager &GetTransactionManager();
  BufferPoolManager &GetBufferPoolManager();
  LogManager &GetLogManager();
  DiskManager &GetDiskManager();
  CheckpointManager &GetCheckpointManager();
  Catalog &GetDBCatalog();
  LockManager &GetLockManager();

  // int64_t NumberOfThreads();
  // static DatabaseInstance &GetDatabase(ClientContext &context);

 private:
  void Initialize(const char *path, DBConfig *config);

 private:
  unique_ptr<BufferPoolManager> buffer_pool_manager_;
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

  ~BusTub();

  shared_ptr<DatabaseInstance> instance_;
};

}  // namespace bustub
