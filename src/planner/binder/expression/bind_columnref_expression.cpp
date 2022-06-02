// #include "catalog/catalog_entry/schema_catalog_entry.h"
#include "common/exception.h"
#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/constant_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "planner/binder.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression_binder.h"
#include "planner/expression_binder/where_binder.h"
#include "type/value_factory.h"

namespace bustub {

unique_ptr<ParsedExpression> ExpressionBinder::QualifyColumnName(const string &column_name, string &error_message) {
  string table_name = binder_.bind_context_.GetMatchingBinding(column_name);
  if (table_name.empty()) {
    error_message = StringUtil::Format("Referenced column \"%s\" not found in FROM clause!", column_name.c_str());
    return nullptr;
  }
  return binder_.bind_context_.CreateColumnReference(table_name, column_name);
}

unique_ptr<ParsedExpression> ExpressionBinder::QualifyColumnName(ColumnRefExpression &colref, string &error_message) {
  uint64_t column_parts = colref.column_names_.size();
  // column names can have an arbitrary amount of dots
  // here is how the resolution works:
  if (column_parts == 1) {
    // no dots (i.e. "part1")
    // -> part1 refers to a column
    // check if we can qualify the column name with the table name
    auto qualified_colref = QualifyColumnName(colref.GetColumnName(), error_message);
    if (qualified_colref) {
      // we could: return it
      return qualified_colref;
    }
    throw NotImplementedException("Colref parsing not done");
    // // we could not! Try creating an implicit struct_pack
    // return CreateStructPack(colref);
  }

  // if (column_parts == 2) {
  //   // one dot (i.e. "part1.part2")
  //   // EITHER:
  //   // -> part1 is a table, part2 is a column
  //   // -> part1 is a column, part2 is a property of that column (i.e. struct_extract)

  //   // first check if part1 is a table
  //   if (binder.HasMatchingBinding(colref.column_names[0], colref.column_names[1], error_message)) {
  //     // it is! return the colref directly
  //     return binder.bind_context.CreateColumnReference(colref.column_names[0], colref.column_names[1]);
  //   }

  //   // otherwise check if we can turn this into a struct extract
  //   auto new_colref = make_unique<ColumnRefExpression>(colref.column_names[0]);
  //   string other_error;
  //   auto qualified_colref = QualifyColumnName(colref.column_names[0], other_error);
  //   if (qualified_colref) {
  //     // we could: create a struct extract
  //     return CreateStructExtract(move(qualified_colref), colref.column_names[1]);
  //   }
  //   // we could not! Try creating an implicit struct_pack
  //   return CreateStructPack(colref);
  // }

  // // two or more dots (i.e. "part1.part2.part3.part4...")
  // // -> part1 is a schema, part2 is a table, part3 is a column name, part4 and beyond are struct fields
  // // -> part1 is a table, part2 is a column name, part3 and beyond are struct fields
  // // -> part1 is a column, part2 and beyond are struct fields

  // // we always prefer the most top-level view
  // // i.e. in case of multiple resolution options, we resolve in order:
  // // -> 1. resolve "part1" as a schema
  // // -> 2. resolve "part1" as a table
  // // -> 3. resolve "part1" as a column

  // unique_ptr<ParsedExpression> result_expr;
  // uint64_t struct_extract_start;
  // // first check if part1 is a schema
  // if (binder_.HasMatchingBinding(colref.column_names_[0], colref.column_names_[1], colref.column_names_[2],
  //                                error_message)) {
  //   // it is! the column reference is "schema.table.column"
  //   // any additional fields are turned into struct_extract calls
  //   result_expr = binder_.bind_context_.CreateColumnReference(colref.column_names_[0], colref.column_names_[1],
  //                                                             colref.column_names_[2]);
  //   struct_extract_start = 3;
  // } else if (binder_.HasMatchingBinding(colref.column_names_[0], colref.column_names_[1], error_message)) {
  //   // part1 is a table
  //   // the column reference is "table.column"
  //   // any additional fields are turned into struct_extract calls
  //   result_expr = binder_.bind_context_.CreateColumnReference(colref.column_names_[0], colref.column_names_[1]);
  //   struct_extract_start = 2;
  // } else {
  //   // part1 could be a column
  //   string col_error;
  //   result_expr = QualifyColumnName(colref.column_names_[0], col_error);
  //   if (!result_expr) {
  //     // it is not! return the error
  //     return nullptr;
  //   }
  //   // it is! add the struct extract calls
  //   struct_extract_start = 1;
  // }
  // // for (uint64_t i = struct_extract_start; i < colref.column_names_.size(); i++) {
  // //   result_expr = CreateStructExtract(move(result_expr), colref.column_names_[i]);
  // // }
  // return result_expr;
  throw NotImplementedException("colref parsing not done");
}

void ExpressionBinder::QualifyColumnNames(unique_ptr<ParsedExpression> &expr) {
  switch (expr->type_) {
    case ExpressionType::COLUMN_REF: {
      auto &colref = (ColumnRefExpression &)*expr;
      string error_message;
      auto new_expr = QualifyColumnName(colref, error_message);
      if (new_expr) {
        if (!expr->alias_.empty()) {
          new_expr->alias_ = expr->alias_;
        }
        expr = move(new_expr);
      }
      break;
    }
    case ExpressionType::POSITIONAL_REFERENCE: {
      throw NotImplementedException("No pos ref");
      // auto &ref = (PositionalReferenceExpression &)*expr;
      // if (ref.alias.empty()) {
      // 	string table_name, column_name;
      // 	auto error = binder.bind_context.BindColumn(ref, table_name, column_name);
      // 	if (error.empty()) {
      // 		ref.alias = column_name;
      // 	}
      // }
      // break;
    }
    default:
      break;
  }
  ParsedExpressionIterator::EnumerateChildren(*expr,
                                              [&](unique_ptr<ParsedExpression> &child) { QualifyColumnNames(child); });
}

void ExpressionBinder::QualifyColumnNames(Binder &binder, unique_ptr<ParsedExpression> &expr) {
  WhereBinder where_binder(binder, binder.context_);
  where_binder.QualifyColumnNames(expr);
}

BindResult ExpressionBinder::BindExpression(ColumnRefExpression &expr, uint64_t depth) {
  if (binder_.GetBindingMode() == BindingMode::EXTRACT_NAMES) {
    const auto val = ValueFactory::GetNullValueByType(TypeId::INTEGER);
    return BindResult(make_unique<BoundConstantExpression>(val));
  }

  string error_message;
  auto qualified_expr = QualifyColumnName(expr, error_message);
  if (!qualified_expr) {
    return BindResult(error_message);
  }
  if (qualified_expr->type_ != ExpressionType::COLUMN_REF) {
    return BindExpression(&qualified_expr, depth, false);
  }
  auto &colref = (ColumnRefExpression &)*qualified_expr;
  assert(colref.column_names_.size() == 2 || colref.column_names_.size() == 3);
  // auto &table_name = colref.column_names_.size() == 3 ? colref.column_names_[1] : colref.column_names_[0];
  // individual column reference
  // resolve to either a base table or a subquery expression
  BindResult result = binder_.bind_context_.BindColumn(colref, depth);
  if (!result.HasError()) {
    BoundColumnReferenceInfo ref;
    ref.name_ = colref.column_names_.back();
    ref.query_location_ = colref.query_location_;
    bound_columns_.push_back(move(ref));
  }
  return result;
}

}  // namespace bustub
