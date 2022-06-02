// #include "planner/operator/logical_limit.h"

// namespace bustub {

// LogicalLimit::LogicalLimit(int64_t limit_val, int64_t offset_val, unique_ptr<Expression> limit,
//                            unique_ptr<Expression> offset)
//     : LogicalOperator(LogicalOperatorType::LOGICAL_LIMIT),
//       limit_val(limit_val),
//       offset_val(offset_val),
//       limit(move(limit)),
//       offset(move(offset)) {}

// vector<ColumnBinding> LogicalLimit::GetColumnBindings() { return children[0]->GetColumnBindings(); }

// void LogicalLimit::ResolveTypes() { types = children[0]->types_; }

// }  // namespace bustub
