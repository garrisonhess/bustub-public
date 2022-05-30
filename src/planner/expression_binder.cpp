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
    if (stored_binder) {
      binder.SetActiveBinder(stored_binder);
    } else {
      binder.PopExpressionBinder();
    }
  }
}

BindResult ExpressionBinder::BindExpression(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression) {
  auto &expr_ref = **expr;
  switch (expr_ref.expression_class) {
    case ExpressionClass::BETWEEN:
      return BindExpression((BetweenExpression &)expr_ref, depth);
    case ExpressionClass::CASE:
      return BindExpression((CaseExpression &)expr_ref, depth);
    case ExpressionClass::CAST:
      return BindExpression((CastExpression &)expr_ref, depth);
    case ExpressionClass::COLLATE:
      return BindExpression((CollateExpression &)expr_ref, depth);
    case ExpressionClass::COLUMN_REF:
      return BindExpression((ColumnRefExpression &)expr_ref, depth);
    case ExpressionClass::COMPARISON:
      return BindExpression((ComparisonExpression &)expr_ref, depth);
    case ExpressionClass::CONJUNCTION:
      return BindExpression((ConjunctionExpression &)expr_ref, depth);
    case ExpressionClass::CONSTANT:
      return BindExpression((ConstantExpression &)expr_ref, depth);
    case ExpressionClass::FUNCTION:
      // binding function expression has extra parameter needed for macro's
      return BindExpression((FunctionExpression &)expr_ref, depth, expr);
    case ExpressionClass::LAMBDA:
      return BindExpression((LambdaExpression &)expr_ref, depth);
    case ExpressionClass::OPERATOR:
      return BindExpression((OperatorExpression &)expr_ref, depth);
    case ExpressionClass::SUBQUERY:
      return BindExpression((SubqueryExpression &)expr_ref, depth);
    case ExpressionClass::PARAMETER:
      return BindExpression((ParameterExpression &)expr_ref, depth);
    case ExpressionClass::POSITIONAL_REFERENCE:
      return BindExpression((PositionalReferenceExpression &)expr_ref, depth);
    default:
      throw NotImplementedException("Unimplemented expression class");
  }
}

bool ExpressionBinder::BindCorrelatedColumns(unique_ptr<ParsedExpression> &expr) {
  // try to bind in one of the outer queries, if the binding error occurred in a subquery
  auto &active_binders = binder.GetActiveBinders();
  // make a copy of the set of binders, so we can restore it later
  auto binders = active_binders;
  active_binders.pop_back();
  uint64_t depth = 1;
  bool success = false;
  while (!active_binders.empty()) {
    auto &next_binder = active_binders.back();
    ExpressionBinder::QualifyColumnNames(next_binder->binder, expr);
    auto bind_result = next_binder->Bind(&expr, depth);
    if (bind_result.empty()) {
      success = true;
      break;
    }
    depth++;
    active_binders.pop_back();
  }
  active_binders = binders;
  return success;
}

void ExpressionBinder::BindChild(unique_ptr<ParsedExpression> &expr, uint64_t depth, string &error) {
  if (expr) {
    string bind_error = Bind(&expr, depth);
    if (error.empty()) {
      error = bind_error;
    }
  }
}

void ExpressionBinder::ExtractCorrelatedExpressions(Binder &binder, Expression &expr) {
  if (expr.type == ExpressionType::BOUND_COLUMN_REF) {
    auto &bound_colref = (BoundColumnRefExpression &)expr;
    if (bound_colref.depth > 0) {
      binder.AddCorrelatedColumn(CorrelatedColumnInfo(bound_colref));
    }
  }
  ExpressionIterator::EnumerateChildren(expr, [&](Expression &child) { ExtractCorrelatedExpressions(binder, child); });
}

bool ExpressionBinder::ContainsType(const Type &type, TypeId target) {
  if (type.id() == target) {
    return true;
  }
  switch (type.id()) {
    case TypeId::STRUCT:
    case TypeId::MAP: {
      auto child_count = StructType::GetChildCount(type);
      for (uint64_t i = 0; i < child_count; i++) {
        if (ContainsType(StructType::GetChildType(type, i), target)) {
          return true;
        }
      }
      return false;
    }
    case TypeId::LIST:
      return ContainsType(ListType::GetChildType(type), target);
    default:
      return false;
  }
}

Type ExpressionBinder::ExchangeType(const Type &type, TypeId target, Type new_type) {
  if (type.id() == target) {
    return new_type;
  }
  switch (type.id()) {
    case TypeId::STRUCT:
    case TypeId::MAP: {
      // we make a copy of the child types of the struct here
      auto child_types = StructType::GetChildTypes(type);
      for (auto &child_type : child_types) {
        child_type.second = ExchangeType(child_type.second, target, new_type);
      }
      return type.id() == TypeId::MAP ? Type::MAP(move(child_types)) : Type::STRUCT(move(child_types));
    }
    case TypeId::LIST:
      return Type::LIST(ExchangeType(ListType::GetChildType(type), target, new_type));
    default:
      return type;
  }
}

bool ExpressionBinder::ContainsNullType(const Type &type) { return ContainsType(type, TypeId::SQLNULL); }

Type ExpressionBinder::ExchangeNullType(const Type &type) { return ExchangeType(type, TypeId::SQLNULL, Type::INTEGER); }

unique_ptr<Expression> ExpressionBinder::Bind(unique_ptr<ParsedExpression> &expr, Type *result_type,
                                              bool root_expression) {
  // bind the main expression
  auto error_msg = Bind(&expr, 0, root_expression);
  if (!error_msg.empty()) {
    // failed to bind: try to bind correlated columns in the expression (if any)
    bool success = BindCorrelatedColumns(expr);
    if (!success) {
      throw BinderException(error_msg);
    }
    auto bound_expr = (BoundExpression *)expr.get();
    ExtractCorrelatedExpressions(binder, *bound_expr->expr);
  }
  assert(expr->expression_class == ExpressionClass::BOUND_EXPRESSION);
  auto bound_expr = (BoundExpression *)expr.get();
  unique_ptr<Expression> result = move(bound_expr->expr);
  if (target_type.id() != TypeId::INVALID) {
    // the binder has a specific target type: add a cast to that type
    result = BoundCastExpression::AddCastToType(move(result), target_type);
  } else {
    if (!binder.can_contain_nulls) {
      // SQL NULL type is only used internally in the binder
      // cast to INTEGER if we encounter it outside of the binder
      if (ContainsNullType(result->return_type)) {
        auto result_type = ExchangeNullType(result->return_type);
        result = BoundCastExpression::AddCastToType(move(result), result_type);
      }
    }
  }
  if (result_type) {
    *result_type = result->return_type;
  }
  return result;
}

string ExpressionBinder::Bind(unique_ptr<ParsedExpression> *expr, uint64_t depth, bool root_expression) {
  // bind the node, but only if it has not been bound yet
  auto &expression = **expr;
  auto alias = expression.alias;
  if (expression.GetExpressionClass() == ExpressionClass::BOUND_EXPRESSION) {
    // already bound, don't bind it again
    return string();
  }
  // bind the expression
  BindResult result = BindExpression(expr, depth, root_expression);
  if (result.HasError()) {
    return result.error;
  } else {
    // successfully bound: replace the node with a BoundExpression
    *expr = make_unique<BoundExpression>(move(result.expression));
    auto be = (BoundExpression *)expr->get();
    assert(be);
    be->alias = alias;
    if (!alias.empty()) {
      be->expr->alias = alias;
    }
    return string();
  }
}

}  // namespace bustub
