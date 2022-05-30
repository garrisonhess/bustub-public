#include "planner/expression_binder.h"

#include "parser/expression/columnref_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "planner/binder.h"
#include "planner/expression/bound_default_expression.h"
#include "planner/expression_iterator.h"

namespace bustub {

ExpressionBinder::ExpressionBinder(Binder &binder, ClientContext &context, bool replace_binder)
    : binder(binder), context(context), stored_binder(nullptr) {
  if (replace_binder) {
    stored_binder = binder.GetActiveBinder();
    binder.SetActiveBinder(this);
  } else {
    binder.PushExpressionBinder(this);
  }
}

ExpressionBinder::~ExpressionBinder() {
  if (binder.HasActiveBinder()) {
    if (stored_binder != nullptr) {
      binder.SetActiveBinder(stored_binder);
    } else {
      binder.PopExpressionBinder();
    }
  }
}

BindResult ExpressionBinder::BindExpression(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression) {
  auto &expr_ref = **expr;
  switch (expr_ref.expression_class_) {
    case ExpressionClass::COLUMN_REF:
      return BindExpression((ColumnRefExpression &)expr_ref, depth);
    case ExpressionClass::CONSTANT:
      return BindExpression((ConstantExpression &)expr_ref, depth);
    case ExpressionClass::FUNCTION:
    case ExpressionClass::LAMBDA:
    case ExpressionClass::OPERATOR:
    case ExpressionClass::SUBQUERY:
    case ExpressionClass::PARAMETER:
    case ExpressionClass::POSITIONAL_REFERENCE:
    case ExpressionClass::BETWEEN:
    case ExpressionClass::CASE:
    case ExpressionClass::CAST:
    case ExpressionClass::COLLATE:
    case ExpressionClass::COMPARISON:
    case ExpressionClass::CONJUNCTION:
      throw NotImplementedException("Unimplemented expression class");
    default:
      throw NotImplementedException("Unimplemented expression class");
  }
}

void ExpressionBinder::BindChild(unique_ptr<ParsedExpression> &expr, uint64_t depth, string &error) {
  if (expr) {
    string bind_error = Bind(&expr, depth);
    if (error.empty()) {
      error = bind_error;
    }
  }
}

// bool ExpressionBinder::ContainsNullType(const Type &type) {
//   return false;
//   // return ContainsType(type, TypeId::SQLNULL);
// }

// Type ExpressionBinder::ExchangeNullType(const Type &type) {
// return ExchangeType(type, TypeId::SQLNULL, Type::INTEGER);
// }

unique_ptr<Expression> ExpressionBinder::Bind(unique_ptr<ParsedExpression> &expr, Type *result_type,
                                              bool root_expression) {
  // bind the main expression
  auto error_msg = Bind(&expr, 0, root_expression);
  // if (!error_msg.empty()) {
  //   // failed to bind: try to bind correlated columns in the expression (if any)
  //   bool success = BindCorrelatedColumns(expr);
  //   if (!success) {
  //     throw Exception(error_msg);
  //   }
  //   auto bound_expr = (BoundExpression *)expr.get();
  //   ExtractCorrelatedExpressions(binder, *bound_expr->expr);
  // }
  assert(expr->expression_class_ == ExpressionClass::BOUND_EXPRESSION);
  auto bound_expr = (BoundExpression *)expr.get();
  unique_ptr<Expression> result = move(bound_expr->expr);
  // if (target_type.id() != TypeId::INVALID) {
  //   // the binder has a specific target type: add a cast to that type
  //   result = BoundCastExpression::AddCastToType(move(result), target_type);
  // } else {
  //   if (!binder.can_contain_nulls) {
  //     // SQL NULL type is only used internally in the binder
  //     // cast to INTEGER if we encounter it outside of the binder
  //     if (ContainsNullType(result->return_type)) {
  //       auto result_type = ExchangeNullType(result->return_type);
  //       result = BoundCastExpression::AddCastToType(move(result), result_type);
  //     }
  //   }
  // }
  if (result_type != nullptr) {
    *result_type = result->return_type_;
  }
  return result;
}

string ExpressionBinder::Bind(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression) {
  // bind the node, but only if it has not been bound yet
  auto &expression = **expr;
  auto alias = expression.alias_;
  if (expression.GetExpressionClass() == ExpressionClass::BOUND_EXPRESSION) {
    // already bound, don't bind it again
    return string();
  }
  // bind the expression
  BindResult result = BindExpression(expr, depth, root_expression);
  if (result.HasError()) {
    return result.error;
  }

  // successfully bound: replace the node with a BoundExpression
  *expr = make_unique<BoundExpression>(move(result.expression));
  auto be = (BoundExpression *)expr->get();
  assert(be);
  be->alias_ = alias;
  if (!alias.empty()) {
    be->expr->alias_ = alias;
  }
  return string();
}

}  // namespace bustub
