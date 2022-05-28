#include "main/query_result.h"

#include <utility>

namespace bustub {
using std::vector;

QueryResult::QueryResult(string error) : error_(std::move(error)) {}

QueryResult::QueryResult(StatementType statement_type, vector<Type> types, vector<string> names)
    : statement_type_(statement_type), types_(std::move(types)), names_(std::move(names)) {}

}  // namespace bustub
