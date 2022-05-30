// // #include "catalog/catalog_entry/schema_catalog_entry.h"
// #include "common/string_util.h"
// #include "parser/expression/columnref_expression.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/parsed_expression_iterator.h"
// #include "planner/binder.h"
// #include "planner/expression/bound_columnref_expression.h"
// #include "planner/expression/bound_constant_expression.h"
// #include "planner/expression_binder.h"
// #include "planner/expression_binder/where_binder.h"

// namespace bustub {

// BindResult ExpressionBinder::BindExpression(ColumnRefExpression &colref_p, uint64_t depth) {
//   // if (binder.GetBindingMode() == BindingMode::EXTRACT_NAMES) {
//   //   return BindResult(make_unique<BoundConstantExpression>(Value(Type::SQLNULL)));
//   // }
//   string error_message;
//   auto expr = QualifyColumnName(colref_p, error_message);
//   if (!expr) {
//     return BindResult(binder.FormatError(colref_p, error_message));
//   }
//   if (expr->type_ != ExpressionType::COLUMN_REF) {
//     return BindExpression(&expr, depth);
//   }
//   auto &colref = (ColumnRefExpression &)*expr;
//   assert(colref.column_names_.size() == 2 || colref.column_names_.size() == 3);
//   auto &table_name = colref.column_names_.size() == 3 ? colref.column_names_[1] : colref.column_names_[0];
//   // individual column reference
//   // resolve to either a base table or a subquery expression
//   // if it was a macro parameter, let macro_binding bind it to the argument
//   BindResult result;
//   if ((binder.macro_binding != nullptr) && table_name == binder.macro_binding->alias) {
//     result = binder.macro_binding->Bind(colref, depth);
//   } else {
//     result = binder.bind_context.BindColumn(colref, depth);
//   }
//   if (!result.HasError()) {
//     BoundColumnReferenceInfo ref;
//     ref.name = colref.column_names_.back();
//     ref.query_location = colref.query_location_;
//     bound_columns.push_back(move(ref));
//   } else {
//     result.error = binder.FormatError(colref_p, result.error);
//   }
//   return result;
// }

// }  // namespace bustub
