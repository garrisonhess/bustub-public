//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// config.h
//
// Identification: src/include/main/config.h
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <chrono>
#include <cstdint>

#include "common/constants.h"

namespace bustub {
// This should be encapsulated properly...
extern bool enable_logging;

struct DBConfig {
  friend class DatabaseInstance;

 public:
  DBConfig() = default;
  ~DBConfig() = default;

  size_t buffer_pool_frames_ = 128;

  /** Cycle detection is performed every CYCLE_DETECTION_INTERVAL milliseconds. */
  std::chrono::milliseconds cycle_detection_interval_ = std::chrono::milliseconds(50);

  /** If ENABLE_LOGGING is true, the log should be flushed to disk every LOG_TIMEOUT. */
  std::chrono::duration<int64_t> log_timeout_ = std::chrono::seconds(1);
};

}  // namespace bustub
