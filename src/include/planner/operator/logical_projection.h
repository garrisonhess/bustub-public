// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // planner/operator/logical_projection.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "planner/logical_operator.h"

// namespace bustub {

// //! LogicalProjection represents the projection list in a SELECT clause
// class LogicalProjection : public LogicalOperator {
//  public:
//   LogicalProjection(uint64_t table_index, vector<unique_ptr<Expression>> select_list);

//   uint64_t table_index;

//  public:
//   vector<ColumnBinding> GetColumnBindings() override;

//  protected:
//   void ResolveTypes() override;
// };
// }  // namespace bustub
