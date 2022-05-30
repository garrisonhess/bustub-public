//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/subquery/flatten_dependent_join.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include "planner/binder.h"
#include "planner/column_binding_map.h"
#include "planner/logical_operator.h"

namespace bustub {

//! The FlattenDependentJoins class is responsible for pushing the dependent join down into the plan to create a
//! flattened subquery
struct FlattenDependentJoins {
  FlattenDependentJoins(Binder &binder, const vector<CorrelatedColumnInfo> &correlated, bool any_join = false);

  //! Detects which Logical Operators have correlated expressions that they are dependent upon, filling the
  //! has_correlated_expressions map.
  bool DetectCorrelatedExpressions(LogicalOperator *op);

  //! Push the dependent join down a LogicalOperator
  unique_ptr<LogicalOperator> PushDownDependentJoin(unique_ptr<LogicalOperator> plan);

  Binder &binder;
  ColumnBinding base_binding;
  uint64_t delim_offset;
  uint64_t data_offset;
  unordered_map<LogicalOperator *, bool> has_correlated_expressions;
  column_binding_map_t<uint64_t> correlated_map;
  column_binding_map_t<uint64_t> replacement_map;
  const vector<CorrelatedColumnInfo> &correlated_columns;
  vector<LogicalType> delim_types;

  bool any_join;

 private:
  unique_ptr<LogicalOperator> PushDownDependentJoinInternal(unique_ptr<LogicalOperator> plan,
                                                            bool &parent_propagate_null_values);
};

}  // namespace bustub
