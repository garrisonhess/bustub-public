// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // planner/operator/logical_insert.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "planner/logical_operator.h"

// namespace bustub {

// //! LogicalInsert represents an insertion of data into a base table
// class LogicalInsert : public LogicalOperator {
//  public:
//   explicit LogicalInsert(TableCatalogEntry *table)
//       : LogicalOperator(LogicalOperatorType::LOGICAL_INSERT), table(table), table_index(0), return_chunk(false) {}

//   vector<vector<unique_ptr<Expression>>> insert_values;
//   //! The insertion map ([table_index -> index in result, or INVALID_INDEX if not specified])
//   vector<uint64_t> column_index_map;
//   //! The expected types for the INSERT statement (obtained from the column types)
//   vector<Type> expected_types;
//   //! The base table to insert into
//   TableCatalogEntry *table;
//   uint64_t table_index;

//  protected:
//   vector<ColumnBinding> GetColumnBindings() override { return {ColumnBinding(0, 0)}; }

//   void ResolveTypes() override { types.emplace_back(Type(TypeId::BIGINT)); }
// };
// }  // namespace bustub
