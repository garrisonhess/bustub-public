//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/group_by_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <set>
#include <vector>
#include "parser/parsed_expression.h"

namespace bustub {
using std::vector;
using GroupingSet = std::set<int64_t>;

class GroupByNode {
 public:
  //! The total set of all group expressions
  vector<std::unique_ptr<ParsedExpression>> group_expressions_;
  //! The different grouping sets as they map to the group expressions
  vector<GroupingSet> grouping_sets_;
};

}  // namespace bustub
