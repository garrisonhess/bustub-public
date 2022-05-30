// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // planner/operator/logical_update.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "planner/logical_operator.h"

// namespace bustub {

// class LogicalUpdate : public LogicalOperator {
//  public:
//   explicit LogicalUpdate(TableCatalogEntry *table)
//       : LogicalOperator(LogicalOperatorType::LOGICAL_UPDATE), table(table), table_index(0), return_chunk(false) {}

//   //! The base table to update
//   TableCatalogEntry *table;
//   //! table catalog index
//   uint64_t table_index;
//   //! if returning option is used, return the update chunk
//   bool return_chunk;
//   vector<uint64_t> columns;
//   vector<unique_ptr<Expression>> bound_defaults;
//   bool update_is_del_and_insert;

//  protected:
//   vector<ColumnBinding> GetColumnBindings() override {
//     if (return_chunk) {
//       return GenerateColumnBindings(table_index, table->columns.size());
//     }
//     return {ColumnBinding(0, 0)};
//   }

//   void ResolveTypes() override {
//     if (return_chunk) {
//       types = table->GetTypes();
//     } else {
//       types.emplace_back(Type::BIGINT);
//     }
//   }
// };
// }  // namespace bustub
