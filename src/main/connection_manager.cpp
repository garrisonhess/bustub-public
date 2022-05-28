#include "main/connection_manager.h"

#include "main/client_context.h"
#include "main/connection.h"

namespace bustub {

ConnectionManager::~ConnectionManager() {
  std::lock_guard<std::mutex> lock(connections_lock_);
  for (auto &conn : connections_) {
    conn->context_->Invalidate();
  }
}

void ConnectionManager::AddConnection(Connection *conn) {
  // D_ASSERT(conn);
  std::lock_guard<std::mutex> lock(connections_lock_);
  connections_.insert(conn);
}

void ConnectionManager::RemoveConnection(Connection *conn) {
  // D_ASSERT(conn);
  std::lock_guard<std::mutex> lock(connections_lock_);
  connections_.erase(conn);
}

}  // namespace bustub
