//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/logical_operator.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "catalog/catalog.h"
#include "common/common.h"
#include "common/enums/logical_operator_type.h"
#include "main/client_context.h"
#include "planner/column_binding.h"
#include "planner/expression.h"
#include "planner/logical_operator_visitor.h"

#include <algorithm>
#include <functional>

namespace bustub {

//! LogicalOperator is the base class of the logical operators present in the
//! logical query tree
class LogicalOperator {
 public:
  explicit LogicalOperator(LogicalOperatorType type) : type_(type) {}
  LogicalOperator(LogicalOperatorType type, vector<unique_ptr<Expression>> expressions)
      : type_(type), expressions_(move(expressions)) {}
  virtual ~LogicalOperator() = default;

  //! The type of the logical operator
  LogicalOperatorType type_;
  //! The set of children of the operator
  vector<unique_ptr<LogicalOperator>> children_;
  //! The set of expressions contained within the operator, if any
  vector<unique_ptr<Expression>> expressions_;
  //! The types returned by this logical operator. Set by calling LogicalOperator::ResolveTypes.
  vector<Type> types_;

 public:
  virtual vector<ColumnBinding> GetColumnBindings() { return {ColumnBinding(0, 0)}; }
  static vector<ColumnBinding> GenerateColumnBindings(uint64_t table_idx, uint64_t column_count);
  static vector<Type> MapTypes(const vector<Type> &types, const vector<uint64_t> &projection_map);
  static vector<ColumnBinding> MapBindings(const vector<ColumnBinding> &bindings,
                                           const vector<uint64_t> &projection_map);

  //! Resolve the types of the logical operator and its children
  void ResolveOperatorTypes();

  virtual string GetName() const;
  virtual string ParamsToString() const;
  virtual string ToString() const;
  void Print();
  //! Debug method: verify that the integrity of expressions & child nodes are maintained
  virtual void Verify();

  void AddChild(unique_ptr<LogicalOperator> child) { children_.push_back(move(child)); }

 protected:
  //! Resolve types for this specific operator
  virtual void ResolveTypes() = 0;
};
}  // namespace bustub
