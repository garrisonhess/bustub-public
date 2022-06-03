// #include "common/case_insensitive_map.h"
// #include "common/helper.h"
// #include "common/string_util.h"
// #include "parser/expression/bound_expression.h"
// #include "parser/expression/columnref_expression.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/tableref/joinref.h"
// #include "planner/binder.h"
// #include "planner/expression_binder/where_binder.h"
// #include "planner/tableref/bound_joinref.h"

// namespace bustub {

// static unique_ptr<ParsedExpression> BindColumn(Binder &binder, ClientContext &context, const string &alias,
//                                                const string &column_name) {
//   auto expr = MakeUniqueBase<ParsedExpression, ColumnRefExpression>(column_name, alias);
//   ExpressionBinder expr_binder(binder, context);
//   auto result = expr_binder.Bind(expr);
//   return make_unique<BoundExpression>(move(result));
// }

// static unique_ptr<ParsedExpression> AddCondition(ClientContext &context, Binder &left_binder, Binder &right_binder,
//                                                  const string &left_alias, const string &right_alias,
//                                                  const string &column_name) {
//   ExpressionBinder expr_binder(left_binder, context);
//   auto left = BindColumn(left_binder, context, left_alias, column_name);
//   auto right = BindColumn(right_binder, context, right_alias, column_name);
//   return make_unique<ComparisonExpression>(ExpressionType::COMPARE_EQUAL, move(left), move(right));
// }

// bool Binder::TryFindBinding(const string &using_column, const string &join_side, string &result) {
//   // for each using column, get the matching binding
//   auto bindings = bind_context_.GetMatchingBindings(using_column);
//   if (bindings.empty()) {
//     return false;
//   }
//   // find the join binding
//   for (auto &binding : bindings) {
//     if (!result.empty()) {
//       string error = "Column name \"";
//       error += using_column;
//       error += "\" is ambiguous: it exists more than once on ";
//       error += join_side;
//       error += " side of join.\nCandidates:";
//       for (auto &binding : bindings) {
//         error += "\n\t";
//         error += binding;
//         error += ".";
//         error += bind_context_.GetActualColumnName(binding, using_column);
//       }
//       throw Exception(error);
//     }
//     result = binding;
//   }
//   return true;
// }

// string Binder::FindBinding(const string &using_column, const string &join_side) {
//   string result;
//   if (!TryFindBinding(using_column, join_side, result)) {
//     throw Exception("Column does not exist on side of join!");
//   }
//   return result;
// }

// static void AddUsingBindings(UsingColumnSet &set, UsingColumnSet *input_set, const string &input_binding) {
//   if (input_set != nullptr) {
//     for (auto &entry : input_set->bindings_) {
//       set.bindings_.insert(entry);
//     }
//   } else {
//     set.bindings_.insert(input_binding);
//   }
// }

// static void SetPrimaryBinding(UsingColumnSet &set, JoinType join_type, const string &left_binding,
//                               const string &right_binding) {
//   switch (join_type) {
//     case JoinType::LEFT:
//     case JoinType::INNER:
//       set.primary_binding_ = left_binding;
//       break;
//     case JoinType::RIGHT:
//       set.primary_binding_ = right_binding;
//       break;
//     default:
//       break;
//   }
// }

// string Binder::RetrieveUsingBinding(Binder &current_binder, UsingColumnSet *current_set, const string &using_column,
//                                     const string &join_side, UsingColumnSet *new_set) {
//   string binding;
//   if (current_set == nullptr) {
//     binding = current_binder.FindBinding(using_column, join_side);
//   } else {
//     binding = current_set->primary_binding_;
//   }
//   return binding;
// }

// unique_ptr<BoundTableRef> Binder::Bind(JoinRef &ref) {
//   auto result = make_unique<BoundJoinRef>();
//   result->left_binder_ = Binder::CreateBinder(context, this);
//   result->right_binder_ = Binder::CreateBinder(context, this);
//   auto &left_binder = *result->left_binder_;
//   auto &right_binder = *result->right_binder_;

//   result->type_ = ref.type_;
//   result->left_ = left_binder.Bind(*ref.left_);
//   result->right_ = right_binder.Bind(*ref.right_);

//   vector<unique_ptr<ParsedExpression>> extra_conditions;
//   vector<string> extra_using_columns;
//   if (ref.is_natural_) {
//     // natural join, figure out which column names are present in both sides of the join
//     // first bind the left hand side and get a list of all the tables and column names
//     case_insensitive_set_t lhs_columns;
//     auto &lhs_binding_list = left_binder.bind_context_.GetBindingsList();
//     for (auto &binding : lhs_binding_list) {
//       for (auto &column_name : binding.second->names) {
//         lhs_columns.insert(column_name);
//       }
//     }
//     // now bind the rhs
//     for (auto &column_name : lhs_columns) {
//       auto right_using_binding = right_binder.bind_context_.GetUsingBinding(column_name);

//       string right_binding;
//       // loop over the set of lhs columns, and figure out if there is a table in the rhs with the same name
//       if (right_using_binding == nullptr) {
//         if (!right_binder.TryFindBinding(column_name, "right", right_binding)) {
//           // no match found for this column on the rhs: skip
//           continue;
//         }
//       }
//       extra_using_columns.push_back(column_name);
//     }
//     if (extra_using_columns.empty()) {
//       // no matching bindings found in natural join: throw an exception
//       string error_msg = "No columns found to join on in NATURAL JOIN.\n";
//       error_msg += "Use CROSS JOIN if you intended for this to be a cross-product.";
//       // gather all left/right candidates
//       string left_candidates;
//       string right_candidates;
//       auto &rhs_binding_list = right_binder.bind_context_.GetBindingsList();
//       for (auto &binding : lhs_binding_list) {
//         for (auto &column_name : binding.second->names_) {
//           if (!left_candidates.empty()) {
//             left_candidates += ", ";
//           }
//           left_candidates += binding.first + "." + column_name;
//         }
//       }
//       for (auto &binding : rhs_binding_list) {
//         for (auto &column_name : binding.second->names_) {
//           if (!right_candidates.empty()) {
//             right_candidates += ", ";
//           }
//           right_candidates += binding.first + "." + column_name;
//         }
//       }
//       error_msg += "\n   Left candidates: " + left_candidates;
//       error_msg += "\n   Right candidates: " + right_candidates;
//       throw Exception(error_msg);
//     }
//   } else if (!ref.using_columns_.empty()) {
//     // USING columns
//     assert(!result->condition_);
//     extra_using_columns = ref.using_columns_;
//   }
//   if (!extra_using_columns.empty()) {
//     vector<UsingColumnSet *> left_using_bindings;
//     vector<UsingColumnSet *> right_using_bindings;
//     for (auto &using_column : extra_using_columns) {
//       // we check if there is ALREADY a using column of the same name in the left and right set
//       // this can happen if we chain USING clauses
//       // e.g. x JOIN y USING (c) JOIN z USING (c)
//       auto left_using_binding = left_binder.bind_context_.GetUsingBinding(using_column);
//       auto right_using_binding = right_binder.bind_context_.GetUsingBinding(using_column);
//       if (left_using_binding == nullptr) {
//         left_binder.bind_context_.GetMatchingBinding(using_column);
//       }
//       if (right_using_binding == nullptr) {
//         right_binder.bind_context_.GetMatchingBinding(using_column);
//       }
//       left_using_bindings.push_back(left_using_binding);
//       right_using_bindings.push_back(right_using_binding);
//     }

//     for (uint64_t i = 0; i < extra_using_columns.size(); i++) {
//       auto &using_column = extra_using_columns[i];
//       string left_binding;
//       string right_binding;

//       auto set = make_unique<UsingColumnSet>();
//       auto left_using_binding = left_using_bindings[i];
//       auto right_using_binding = right_using_bindings[i];
//       left_binding = RetrieveUsingBinding(left_binder, left_using_binding, using_column, "left", set.get());
//       right_binding = RetrieveUsingBinding(right_binder, right_using_binding, using_column, "right", set.get());

//       extra_conditions.push_back(
//           AddCondition(context_, left_binder, right_binder, left_binding, right_binding, using_column));

//       AddUsingBindings(*set, left_using_binding, left_binding);
//       AddUsingBindings(*set, right_using_binding, right_binding);
//       SetPrimaryBinding(*set, ref.type_, left_binding, right_binding);
//       bind_context_.TransferUsingBinding(left_binder.bind_context_, left_using_binding, set.get(), left_binding,
//                                         using_column);
//       bind_context_.TransferUsingBinding(right_binder.bind_context_, right_using_binding, set.get(), right_binding,
//                                         using_column);
//       AddUsingBindingSet(move(set));
//     }
//   }

//   bind_context_.AddContext(move(left_binder.bind_context_));
//   bind_context_.AddContext(move(right_binder.bind_context_));
//   MoveCorrelatedExpressions(left_binder);
//   MoveCorrelatedExpressions(right_binder);
//   for (auto &condition : extra_conditions) {
//     if (ref.condition_) {
//       ref.condition_ = make_unique<ConjunctionExpression>(ExpressionType::CONJUNCTION_AND, move(ref.condition_),
//       move(condition));
//     } else {
//       ref.condition_ = move(condition);
//     }
//   }
//   if (ref.condition_) {
//     WhereBinder binder(*this, context_);
//     result->condition_ = binder.Bind(ref.condition_);
//   }
//   assert(result->condition_);
//   return move(result);
// }

// }  // namespace bustub
