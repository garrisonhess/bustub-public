//===----------------------------------------------------------------------===//
//                         BusTub
//
// main/connection_manager.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_set>

#include <mutex>

namespace bustub {

class BusTub;
class Connection;

class ConnectionManager {
 public:
  ~ConnectionManager() = default;

  void AddConnection(Connection *conn);
  void RemoveConnection(Connection *conn);

  template <class T>
  void Scan(T &&callback) {
    // lock the catalog set
    std::lock_guard<std::mutex> lock(connections_lock_);
    for (auto &conn : connections_) {
      callback(conn);
    }
  }

 private:
  std::mutex connections_lock_;
  std::unordered_set<Connection *> connections_;
};

}  // namespace bustub
