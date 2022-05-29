//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/common/enums/order_type.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>

namespace bustub {

enum class OrderType : uint8_t { INVALID = 0, ORDER_DEFAULT = 1, ASCENDING = 2, DESCENDING = 3 };
enum class OrderByNullType : uint8_t { INVALID = 0, ORDER_DEFAULT = 1, NULLS_FIRST = 2, NULLS_LAST = 3 };

}  // namespace bustub
