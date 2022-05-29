//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/enums/pending_execution_result.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/constants.h"

namespace bustub {

enum class PendingExecutionResult : uint8_t { RESULT_READY, RESULT_NOT_READY, EXECUTION_ERROR };

}  // namespace bustub
