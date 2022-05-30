#include "planner/expression/bound_cast_expression.h"
#include "planner/expression/bound_default_expression.h"
#include "planner/expression/bound_parameter_expression.h"

namespace bustub {

BoundCastExpression::BoundCastExpression(unique_ptr<Expression> child_p, Type target_type_p, bool try_cast_p)
    : Expression(ExpressionType::OPERATOR_CAST, ExpressionClass::BOUND_CAST, move(target_type_p)),
      child(move(child_p)),
      try_cast(try_cast_p) {}

unique_ptr<Expression> BoundCastExpression::AddCastToType(unique_ptr<Expression> expr, const Type &target_type,
                                                          bool try_cast) {
  D_ASSERT(expr);
  if (expr->expression_class == ExpressionClass::BOUND_PARAMETER) {
    auto &parameter = (BoundParameterExpression &)*expr;
    parameter.return_type = target_type;
  } else if (expr->expression_class == ExpressionClass::BOUND_DEFAULT) {
    auto &def = (BoundDefaultExpression &)*expr;
    def.return_type = target_type;
  } else if (expr->return_type != target_type) {
    auto &expr_type = expr->return_type;
    if (target_type.id() == TypeId::LIST && expr_type.id() == TypeId::LIST) {
      auto &target_list = ListType::GetChildType(target_type);
      auto &expr_list = ListType::GetChildType(expr_type);
      if (target_list.id() == TypeId::ANY || expr_list == target_list) {
        return expr;
      }
    }
    return make_unique<BoundCastExpression>(move(expr), target_type, try_cast);
  }
  return expr;
}

bool BoundCastExpression::CastIsInvertible(const Type &source_type, const Type &target_type) {
  if (source_type.id() == TypeId::BOOLEAN || target_type.id() == TypeId::BOOLEAN) {
    return false;
  }
  if (source_type.id() == TypeId::FLOAT || target_type.id() == TypeId::FLOAT) {
    return false;
  }
  if (source_type.id() == TypeId::DOUBLE || target_type.id() == TypeId::DOUBLE) {
    return false;
  }
  if (source_type.id() == TypeId::DECIMAL || target_type.id() == TypeId::DECIMAL) {
    uint8_t source_width, target_width;
    uint8_t source_scale, target_scale;
    // cast to or from decimal
    // cast is only invertible if the cast is strictly widening
    if (!source_type.GetDecimalProperties(source_width, source_scale)) {
      return false;
    }
    if (!target_type.GetDecimalProperties(target_width, target_scale)) {
      return false;
    }
    if (target_scale < source_scale) {
      return false;
    }
    return true;
  }
  if (source_type.id() == TypeId::TIMESTAMP || source_type.id() == TypeId::TIMESTAMP_TZ) {
    switch (target_type.id()) {
      case TypeId::DATE:
      case TypeId::TIME:
      case TypeId::TIME_TZ:
        return false;
      default:
        break;
    }
  }
  if (source_type.id() == TypeId::VARCHAR) {
    switch (target_type.id()) {
      case TypeId::DATE:
      case TypeId::TIME:
      case TypeId::TIMESTAMP:
      case TypeId::TIMESTAMP_NS:
      case TypeId::TIMESTAMP_MS:
      case TypeId::TIMESTAMP_SEC:
      case TypeId::TIME_TZ:
      case TypeId::TIMESTAMP_TZ:
        return true;
      default:
        return false;
    }
  }
  if (target_type.id() == TypeId::VARCHAR) {
    switch (source_type.id()) {
      case TypeId::DATE:
      case TypeId::TIME:
      case TypeId::TIMESTAMP:
      case TypeId::TIMESTAMP_NS:
      case TypeId::TIMESTAMP_MS:
      case TypeId::TIMESTAMP_SEC:
      case TypeId::TIME_TZ:
      case TypeId::TIMESTAMP_TZ:
        return true;
      default:
        return false;
    }
  }
  return true;
}

string BoundCastExpression::ToString() const {
  return (try_cast ? "TRY_CAST(" : "CAST(") + child->GetName() + " AS " + return_type.ToString() + ")";
}

bool BoundCastExpression::Equals(const BaseExpression *other_p) const {
  if (!Expression::Equals(other_p)) {
    return false;
  }
  auto other = (BoundCastExpression *)other_p;
  if (!Expression::Equals(child.get(), other->child.get())) {
    return false;
  }
  if (try_cast != other->try_cast) {
    return false;
  }
  return true;
}

unique_ptr<Expression> BoundCastExpression::Copy() {
  auto copy = make_unique<BoundCastExpression>(child->Copy(), return_type, try_cast);
  copy->CopyProperties(*this);
  return move(copy);
}

}  // namespace bustub
