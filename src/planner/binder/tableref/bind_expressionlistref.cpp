#include "common/common.h"
#include "parser/tableref/expressionlistref.h"
#include "planner/binder.h"
#include "planner/expression_binder/insert_binder.h"
#include "planner/tableref/bound_expressionlistref.h"

namespace bustub {

unique_ptr<BoundTableRef> Binder::Bind(ExpressionListRef &expr) {
  auto result = make_unique<BoundExpressionListRef>();
  result->types = expr.expected_types_;
  result->names = expr.expected_names_;
  // bind value list
  InsertBinder binder(*this, context);
  binder.target_type = Type(TypeId::INVALID);
  for (auto &expression_list : expr.values_) {
    if (result->names.empty()) {
      // no names provided, generate them
      for (uint64_t val_idx = 0; val_idx < expression_list.size(); val_idx++) {
        result->names.push_back("col" + std::to_string(val_idx));
      }
    }

    vector<unique_ptr<Expression>> list;
    for (uint64_t val_idx = 0; val_idx < expression_list.size(); val_idx++) {
      if (!result->types.empty()) {
        assert(result->types.size() == expression_list.size());
        binder.target_type = result->types[val_idx];
      }
      auto expr = binder.Bind(expression_list[val_idx]);
      list.push_back(move(expr));
    }
    result->values.push_back(move(list));
  }
  if (result->types.empty() && !expr.values_.empty()) {
    // there are no types specified
    // we have to figure out the result types
    // for each column, we iterate over all of the expressions and select the max logical type
    // we initialize all types to SQLNULL
    result->types.resize(expr.values_[0].size(), Type::SQLNULL);
    // now loop over the lists and select the max logical type
    for (uint64_t list_idx = 0; list_idx < result->values.size(); list_idx++) {
      auto &list = result->values[list_idx];
      for (uint64_t val_idx = 0; val_idx < list.size(); val_idx++) {
        result->types[val_idx] = Type::MaxType(result->types[val_idx], list[val_idx]->return_type_);
      }
    }
    // finally do another loop over the expressions and add casts where required
    for (auto &list : result->values) {
      for (uint64_t val_idx = 0; val_idx < list.size(); val_idx++) {
        list[val_idx] = BoundCastExpression::AddCastToType(move(list[val_idx]), result->types[val_idx]);
      }
    }
  }
  result->bind_index = GenerateTableIndex();
  bind_context.AddGenericBinding(result->bind_index, expr.alias_, result->names, result->types);
  return move(result);
}

}  // namespace bustub
