#include "common/common.h"
#include "common/exception.h"
#include "parser/tableref/expressionlistref.h"
#include "planner/binder.h"
#include "planner/expression_binder/insert_binder.h"
#include "planner/tableref/bound_expressionlistref.h"

namespace bustub {

unique_ptr<BoundTableRef> Binder::Bind(ExpressionListRef &ref) {
  auto result = make_unique<BoundExpressionListRef>();
  result->types_ = ref.expected_types_;
  result->names_ = ref.expected_names_;
  // // bind value list
  // InsertBinder binder(*this, context_);
  // binder.target_type_ = Type(TypeId::INVALID);
  // for (auto &expression_list : ref.values_) {
  //   if (result->names_.empty()) {
  //     // no names provided, generate them
  //     for (uint64_t val_idx = 0; val_idx < expression_list.size(); val_idx++) {
  //       result->names_.push_back("col" + std::to_string(val_idx));
  //     }
  //   }

  //   vector<unique_ptr<Expression>> list;
  //   for (uint64_t val_idx = 0; val_idx < expression_list.size(); val_idx++) {
  //     if (!result->types_.empty()) {
  //       assert(result->types_.size() == expression_list.size());
  //       binder.target_type_ = result->types_[val_idx];
  //     }
  //     auto expr = binder.Bind(expression_list[val_idx]);
  //     list.push_back(move(expr));
  //   }
  //   result->values_.push_back(move(list));
  // }
  // if (result->types_.empty() && !ref.values_.empty()) {
  //   // there are no types specified
  //   // we have to figure out the result types
  //   // for each column, we iterate over all of the expressions and select the max logical type
  //   // we initialize all types to SQLNULL
  //   result->types_.resize(ref.values_[0].size(), Type::SQLNULL);
  //   // now loop over the lists and select the max logical type
  //   for (auto & list : result->values_) {
  //      for (uint64_t val_idx = 0; val_idx < list.size(); val_idx++) {
  //       result->types_[val_idx] = Type::MaxType(result->types_[val_idx], list[val_idx]->return_type_);
  //     }
  //   }
  //   // finally do another loop over the expressions and add casts where required
  //   for (auto &list : result->values_) {
  //     for (uint64_t val_idx = 0; val_idx < list.size(); val_idx++) {
  //       list[val_idx] = BoundCastExpression::AddCastToType(move(list[val_idx]), result->types_[val_idx]);
  //     }
  //   }
  // }
  // HACK ALERT: calling everything ints for now
  result->types_.resize(ref.values_[0].size(), Type(TypeId::INTEGER));

  result->bind_index_ = GenerateTableIndex();
  bind_context_.AddGenericBinding(result->bind_index_, ref.alias_, result->names_, result->types_);
  return result;
}

}  // namespace bustub
