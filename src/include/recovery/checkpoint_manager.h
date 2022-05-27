//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// checkpoint_manager.h
//
// Identification: src/include/recovery/checkpoint_manager.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "buffer/buffer_pool_manager.h"
#include "concurrency/transaction_manager.h"
#include "main/database.h"
#include "recovery/log_manager.h"

namespace bustub {
class DatabaseInstance;

/**
 * CheckpointManager creates consistent checkpoints by blocking all other transactions temporarily.
 */
class CheckpointManager {
 public:
  explicit CheckpointManager(DatabaseInstance &db);

  ~CheckpointManager() = default;

  void BeginCheckpoint();
  void EndCheckpoint();

 private:
  DatabaseInstance &db_;
};

}  // namespace bustub
