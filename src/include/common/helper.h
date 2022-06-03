//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/helper.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include "common/constants.h"

namespace bustub {

using std::make_unique;
template <typename S, typename T, typename... Args>
unique_ptr<S> MakeUniqueBase(Args &&...args) {
  return unique_ptr<S>(new T(std::forward<Args>(args)...));
}

template <typename T, typename S>
unique_ptr<S> UniquePtrCast(unique_ptr<T> src) {
  return unique_ptr<S>(static_cast<S *>(src.release()));
}

}  // namespace bustub
