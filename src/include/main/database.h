//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/main/database.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

// #include "main/config.h"
// #include "main/extension.h"

#include <memory>

namespace bustub {
using std::string;
using std::unique_ptr;

// class StorageManager;
// class Catalog;
// class TransactionManager;
// class ConnectionManager;
// class FileSystem;
// class TaskScheduler;
// class ObjectCache;

//! The database object. This object holds the catalog and all the
//! database-specific meta information.
class Connection;
class BusTub {
 public:
  explicit BusTub(const string &path);
  //   BusTub(const string &path, DBConfig *config = nullptr);

  ~BusTub() = default;

  //   DBConfig config;

  //   unique_ptr<StorageManager> storage;
  //   unique_ptr<Catalog> catalog;
  //   unique_ptr<TransactionManager> transaction_manager;
  //   unique_ptr<TaskScheduler> scheduler;
  //   unique_ptr<ConnectionManager> connection_manager;
  //   unique_ptr<ObjectCache> object_cache;

 public:
  //   template <class T>
  //   void LoadExtension() {
  //     T extension;
  //     extension.Load(*this);
  //   }

  //   FileSystem &GetFileSystem();

  //   uint64_t NumberOfThreads();
  //   static const char *SourceID();
  //   static const char *LibraryVersion();

  //  private:
  //   void Configure(DBConfig &config);
};

}  // namespace bustub
