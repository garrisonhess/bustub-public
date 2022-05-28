#include "main/query_result.h"

namespace bustub {
QueryResult::QueryResult(string error) : error_(std::move(error)) {}

QueryResult::QueryResult() = default;

}  // namespace bustub
