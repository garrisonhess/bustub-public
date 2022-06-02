#include "planner/operator/logical_limit.h"

namespace bustub {

LogicalLimit::LogicalLimit(int64_t limit_val, int64_t offset_val, unique_ptr<Expression> limit,
                           unique_ptr<Expression> offset)
    : LogicalOperator(LogicalOperatorType::LOGICAL_LIMIT),
      limit_val_(limit_val),
      offset_val_(offset_val),
      limit_(move(limit)),
      offset_(move(offset)) {}

vector<ColumnBinding> LogicalLimit::GetColumnBindings() { return children_[0]->GetColumnBindings(); }

void LogicalLimit::ResolveTypes() { types_ = children_[0]->types_; }

}  // namespace bustub
