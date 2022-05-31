#include "common/enums/join_type.h"

namespace bustub {

string JoinTypeToString(JoinType type) {
  switch (type) {
    case JoinType::LEFT:
      return "LEFT";
    case JoinType::RIGHT:
      return "RIGHT";
    case JoinType::INNER:
      return "INNER";
    case JoinType::OUTER:
      return "FULL";
    case JoinType::INVALID:  // LCOV_EXCL_START
      break;
  }
  return "INVALID";
}

bool IsLeftOuterJoin(JoinType type) { return type == JoinType::LEFT || type == JoinType::OUTER; }

bool IsRightOuterJoin(JoinType type) { return type == JoinType::OUTER || type == JoinType::RIGHT; }

}  // namespace bustub
