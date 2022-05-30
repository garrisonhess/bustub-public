//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/case_insensitive_map.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "common/string_util.h"

namespace bustub {
using std::string;
using std::unordered_map;
using std::unordered_set;

struct CaseInsensitiveStringHashFunction {
  uint64_t operator()(const string &str) const {
    std::hash<string> hasher;
    return hasher(StringUtil::Lower(str));
  }
};

struct CaseInsensitiveStringEquality {
  bool operator()(const string &a, const string &b) const { return StringUtil::Lower(a) == StringUtil::Lower(b); }
};

template <typename T>
using case_insensitive_map_t =
    unordered_map<string, T, CaseInsensitiveStringHashFunction, CaseInsensitiveStringEquality>;

using case_insensitive_set_t = unordered_set<string, CaseInsensitiveStringHashFunction, CaseInsensitiveStringEquality>;

}  // namespace bustub
