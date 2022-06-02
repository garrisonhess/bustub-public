//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/base_expression.h"
#include "type/type.h"

namespace bustub {
//!  The Expression class represents a bound Expression with a return type
class Expression : public BaseExpression {
 public:
  Expression(ExpressionType type, ExpressionClass expression_class, const Type &return_type);
  ~Expression() override;

  //! The return type of the expression
  Type return_type_;

 public:
  bool IsAggregate() const override;
  bool IsWindow() const override;
  bool HasSubquery() const override;
  bool IsScalar() const override;
  bool HasParameter() const override;
  virtual bool HasSideEffects() const;
  virtual bool PropagatesNullValues() const;
  virtual bool IsFoldable() const;

  hash_t Hash() const override;

  bool Equals(const BaseExpression *other) const override {
    if (!BaseExpression::Equals(other)) {
      return false;
    }
    return return_type_.GetTypeId() == ((Expression *)other)->return_type_.GetTypeId();
  }

  static bool Equals(Expression *left, Expression *right) {
    return BaseExpression::Equals((BaseExpression *)left, (BaseExpression *)right);
  }

  //! Create a copy of this expression
  virtual unique_ptr<Expression> Copy() = 0;

 protected:
  //! Copy base Expression properties from another expression to this one,
  //! used in Copy method
  void CopyProperties(Expression &other) {
    type_ = other.type_;
    expression_class_ = other.expression_class_;
    alias_ = other.alias_;
    return_type_ = other.return_type_;
  }
};

}  // namespace bustub
