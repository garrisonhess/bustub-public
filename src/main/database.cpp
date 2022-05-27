#include "main/database.h"
#include <memory>

#include "catalog/catalog.h"
// #include "common/file_system.h"
// #include "main/client_context.h"
// #include "main/connection_manager.h"
// #include "parallel/task_scheduler.h"
// #include "storage/storage_manager.h"
// #include "storage/object_cache.h"
// #include "transaction/transaction_manager.h"

namespace bustub {
using std::make_unique;

// BusTubConfig::~DBConfig() {
// }

BusTub::BusTub(const string &path) {
  // if (new_config) {
  // 	// user-supplied configuration
  // 	Configure(*new_config);
  // } else {
  // 	// default configuration
  // 	DBConfig config;
  // 	Configure(config);
  // }
  // if (config.temporary_directory.empty() && path) {
  // 	// no directory specified: use default temp path
  // 	config.temporary_directory = string(path) + ".tmp";

  // 	// special treatment for in-memory mode
  // 	if (strcmp(path, ":memory:") == 0) {
  // 		config.temporary_directory = ".tmp";
  // 	}
  // }
  // if (new_config && !new_config->use_temporary_directory) {
  // 	// temporary directories explicitly disabled
  // 	config.temporary_directory = string();
  // }

//   storage =
//       make_unique<StorageManager>(*this, path ? string(path) : string(), config.access_mode == AccessMode::READ_ONLY);
//   catalog = make_unique<Catalog>(*storage);
//   transaction_manager = make_unique<TransactionManager>(*storage);
// //   scheduler = make_unique<TaskScheduler>();
//   connection_manager = make_unique<ConnectionManager>();
// //   object_cache = make_unique<ObjectCache>();

//   // initialize the database
//   storage->Initialize();
}

// BusTub::BusTub(const string &path, DBConfig *config) : DuckDB(path.c_str(), config) {
// }

// FileSystem &DuckDB::GetFileSystem() {
// 	return *config.file_system;
// }

// void DuckDB::Configure(DBConfig &new_config) {
// 	if (new_config.access_mode != AccessMode::UNDEFINED) {
// 		config.access_mode = new_config.access_mode;
// 	} else {
// 		config.access_mode = AccessMode::READ_WRITE;
// 	}
// 	if (new_config.file_system) {
// 		config.file_system = move(new_config.file_system);
// 	} else {
// 		config.file_system = make_unique<FileSystem>();
// 	}
// 	if (config.maximum_memory == (idx_t)-1) {
// 		config.maximum_memory = config.file_system->GetAvailableMemory() * 8 / 10;
// 	} else {
// 		config.maximum_memory = new_config.maximum_memory;
// 	}
// 	config.checkpoint_only = new_config.checkpoint_only;
// 	config.checkpoint_wal_size = new_config.checkpoint_wal_size;
// 	config.use_direct_io = new_config.use_direct_io;
// 	config.maximum_memory = new_config.maximum_memory;
// 	config.temporary_directory = new_config.temporary_directory;
// 	config.collation = new_config.collation;
// 	config.default_order_type = new_config.default_order_type;
// 	config.default_null_order = new_config.default_null_order;
// 	config.enable_copy = new_config.enable_copy;
// }

// DBConfig &DBConfig::GetConfig(ClientContext &context) {
// 	return context.db.config;
// }

// idx_t DuckDB::NumberOfThreads() {
// 	return scheduler->NumberOfThreads();
// }

}  // namespace bustub
