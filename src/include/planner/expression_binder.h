//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression_binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "common/exception.h"
#include "parser/expression/bound_expression.h"
#include "parser/parsed_expression.h"
#include "parser/tokens.h"
#include "planner/expression.h"

#include <unordered_map>

namespace bustub {

class Binder;
class ClientContext;
class QueryNode;

class ScalarFunctionCatalogEntry;
class AggregateFunctionCatalogEntry;
class ScalarMacroCatalogEntry;
class CatalogEntry;
class SimpleFunction;

struct BoundColumnReferenceInfo {
  string name;
  uint64_t query_location;
};

struct BindResult {
  BindResult() {}
  explicit BindResult(string error) : error(error) {}
  explicit BindResult(unique_ptr<Expression> expr) : expression(move(expr)) {}

  bool HasError() { return !error.empty(); }

  unique_ptr<Expression> expression;
  string error;
};

class ExpressionBinder {
 public:
  ExpressionBinder(Binder &binder, ClientContext &context, bool replace_binder);
  virtual ~ExpressionBinder();

  //! The target type that should result from the binder. If the result is not of this type, a cast to this type will
  //! be added. Defaults to INVALID.
  Type target_type;

 public:
  unique_ptr<Expression> Bind(unique_ptr<ParsedExpression> &expr, Type *result_type = nullptr,
                              bool root_expression = true);

  //! Returns whether or not any columns have been bound by the expression binder
  bool HasBoundColumns() { return !bound_columns.empty(); }
  const vector<BoundColumnReferenceInfo> &GetBoundColumns() { return bound_columns; }

  string Bind(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression = false);

  unique_ptr<ParsedExpression> CreateStructExtract(unique_ptr<ParsedExpression> base, string field_name);
  unique_ptr<ParsedExpression> CreateStructPack(ColumnRefExpression &colref);
  BindResult BindQualifiedColumnName(ColumnRefExpression &colref, const string &table_name);

  unique_ptr<ParsedExpression> QualifyColumnName(const string &column_name, string &error_message);
  unique_ptr<ParsedExpression> QualifyColumnName(ColumnRefExpression &colref, string &error_message);

  // Bind table names to ColumnRefExpressions
  void QualifyColumnNames(unique_ptr<ParsedExpression> &expr);
  static void QualifyColumnNames(Binder &binder, unique_ptr<ParsedExpression> &expr);

  void BindChild(unique_ptr<ParsedExpression> &expr, uint64_t depth, string &error);

  static bool ContainsType(const Type &type, TypeId target);
  static Type ExchangeType(const Type &type, TypeId target, Type new_type);

  //! Bind the given expresion. Unlike Bind(), this does *not* mute the given ParsedExpression.
  //! Exposed to be used from sub-binders that aren't subclasses of ExpressionBinder.
  virtual BindResult BindExpression(unique_ptr<ParsedExpression> *expr_ptr, uint64_t depth, bool root_expression);

 protected:
  BindResult BindExpression(ColumnRefExpression &expr, uint64_t depth);
  BindResult BindExpression(ConstantExpression &expr, uint64_t depth);

 protected:
  Binder &binder;
  ClientContext &context;
  ExpressionBinder *stored_binder;
  vector<BoundColumnReferenceInfo> bound_columns;
};

}  // namespace bustub
