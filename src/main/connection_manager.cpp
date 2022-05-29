#include "main/connection_manager.h"

#include "main/client_context.h"
#include "main/connection.h"

namespace bustub {

void ConnectionManager::AddConnection(Connection *conn) {
  assert(conn);
  connections_.insert(conn);
}

void ConnectionManager::RemoveConnection(Connection *conn) {
  assert(conn);
  connections_.erase(conn);
}

}  // namespace bustub
