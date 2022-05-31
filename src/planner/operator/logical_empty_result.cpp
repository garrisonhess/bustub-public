#include "planner/operator/logical_empty_result.h"

namespace bustub {

LogicalEmptyResult::LogicalEmptyResult(unique_ptr<LogicalOperator> op)
    : LogicalOperator(LogicalOperatorType::LOGICAL_EMPTY_RESULT) {
  this->bindings_ = op->GetColumnBindings();

  op->ResolveOperatorTypes();
  this->return_types_ = op->types_;
}

}  // namespace bustub
