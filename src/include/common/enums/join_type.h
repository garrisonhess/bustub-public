//===----------------------------------------------------------------------===//
//                         BusTub
//
// common/enums/join_type.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.h"

#include <string>

namespace bustub {

//===--------------------------------------------------------------------===//
// Join Types
//===--------------------------------------------------------------------===//
enum class JoinType : uint8_t {
  INVALID = 0,  // invalid join type
  LEFT = 1,     // left
  RIGHT = 2,    // right
  INNER = 3,    // inner
  OUTER = 4,    // outer
};

//! Convert join type to string
std::string JoinTypeToString(JoinType type);

//! True if join is left or full outer join
bool IsLeftOuterJoin(JoinType type);

//! True if join is rght or full outer join
bool IsRightOuterJoin(JoinType type);

}  // namespace bustub
