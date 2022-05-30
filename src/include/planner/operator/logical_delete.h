// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // planner/operator/logical_delete.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "planner/logical_operator.h"

// namespace bustub {

// class LogicalDelete : public LogicalOperator {
//  public:
//   explicit LogicalDelete(TableCatalogEntry *table)
//       : LogicalOperator(LogicalOperatorType::LOGICAL_DELETE), table(table), table_index(0), return_chunk(false) {}

//   TableCatalogEntry *table;
//   uint64_t table_index;

//  protected:
//   vector<ColumnBinding> GetColumnBindings() override { return {ColumnBinding(0, 0)}; }

//   void ResolveTypes() override { types.emplace_back(Type(TypeId::BIGINT)); }
// };
// }  // namespace bustub
