

#include "main/database.h"
#include "buffer/buffer_pool_manager_instance.h"
#include "catalog/catalog.h"
#include "recovery/checkpoint_manager.h"
#include "storage/disk/disk_manager.h"

#include <memory>

namespace bustub {
class BufferPoolManagerInstance;

using std::make_unique;

TransactionManager &DatabaseInstance::GetTransactionManager() { return *transaction_manager_; }

BufferPoolManagerInstance &DatabaseInstance::GetBufferPoolManager() { return *buffer_pool_manager_; }

LogManager &DatabaseInstance::GetLogManager() { return *log_manager_; }

DiskManager &DatabaseInstance::GetDiskManager() { return *disk_manager_; }

CheckpointManager &DatabaseInstance::GetCheckpointManager() { return *checkpoint_manager_; }

Catalog &DatabaseInstance::GetCatalog() { return *catalog_; }

LockManager &DatabaseInstance::GetLockManager() { return *lock_manager_; }

// The initialization must occur in DB-bootstrap order.
void DatabaseInstance::Initialize(const char *path, DBConfig *config) {


  int default_num_frames = 1234;
  disk_manager_ = make_unique<DiskManager>(path);
  buffer_pool_manager_ = make_unique<BufferPoolManagerInstance>(*this, default_num_frames);
  checkpoint_manager_ = make_unique<CheckpointManager>(*this);
  catalog_ = make_unique<Catalog>(*this);
  log_manager_ = make_unique<LogManager>(*this->disk_manager_);
  lock_manager_ = make_unique<LockManager>();
  transaction_manager_ = make_unique<TransactionManager>(*this);
}

BusTub::BusTub(const string &path, DBConfig *config) : BusTub(path.c_str(), config) {}

BusTub::BusTub(const char *path, DBConfig *config) : instance_(make_shared<DatabaseInstance>()) {
  if (config == nullptr){
    config = new DBConfig();
  }
  
  instance_->Initialize(path, config);
}

// BusTub::BusTub(DatabaseInstance &instance) : instance_(instance.shared_from_this()) {}

TransactionManager &TransactionManager::Get(DatabaseInstance &db) { return db.GetTransactionManager(); }

BufferPoolManagerInstance &BufferPoolManagerInstance::Get(DatabaseInstance &db) { return db.GetBufferPoolManager(); }

LogManager &LogManager::Get(DatabaseInstance &db) { return db.GetLogManager(); }

DiskManager &DiskManager::Get(DatabaseInstance &db) { return db.GetDiskManager(); }

CheckpointManager &CheckpointManager::Get(DatabaseInstance &db) { return db.GetCheckpointManager(); }

Catalog &Catalog::Get(DatabaseInstance &db) { return db.GetCatalog(); }

LockManager &LockManager::Get(DatabaseInstance &db) { return db.GetLockManager(); }





}  // namespace bustub
