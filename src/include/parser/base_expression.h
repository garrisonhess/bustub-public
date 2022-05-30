//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/base_expression.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.h"
#include "common/hash_util.h"
#include "type/expression_type.h"

namespace bustub {

//!  The BaseExpression class is a base class that can represent any expression
//!  part of a SQL statement.
class BaseExpression {
 public:
  //! Create an Expression
  BaseExpression(ExpressionType type, ExpressionClass expression_class)
      : type_(type), expression_class_(expression_class) {}
  virtual ~BaseExpression() = default;

  //! Returns the type of the expression
  ExpressionType GetExpressionType() const { return type_; }
  //! Returns the class of the expression
  ExpressionClass GetExpressionClass() const { return expression_class_; }

  //! Type of the expression
  ExpressionType type_;
  //! The expression class of the node
  ExpressionClass expression_class_;
  //! The alias of the expression,
  std::string alias_;

 public:
  //! Returns true if this expression is an aggregate or not.
  /*!
   Examples:

   (1) SUM(a) + 1 -- True

   (2) a + 1 -- False
   */
  virtual bool IsAggregate() const = 0;
  //! Returns true if the expression has a window function or not
  virtual bool IsWindow() const = 0;
  //! Returns true if the query contains a subquery
  virtual bool HasSubquery() const = 0;
  //! Returns true if expression does not contain a group ref or col ref or parameter
  virtual bool IsScalar() const = 0;
  //! Returns true if the expression has a parameter
  virtual bool HasParameter() const = 0;

  //! Get the name of the expression
  virtual std::string GetName() const;
  //! Convert the Expression to a String
  virtual std::string ToString() const = 0;
  //! Print the expression to stdout
  void Print() const;

  //! Creates a hash value of this expression. It is important that if two expressions are identical (i.e.
  //! Expression::Equals() returns true), that their hash value is identical as well.
  virtual hash_t Hash() const = 0;
  //! Returns true if this expression is equal to another expression
  virtual bool Equals(const BaseExpression *other) const;

  static bool Equals(BaseExpression *left, BaseExpression *right) {
    if (left == right) {
      return true;
    }
    if ((left == nullptr) || (right == nullptr)) {
      return false;
    }
    return left->Equals(right);
  }
  bool operator==(const BaseExpression &rhs) { return this->Equals(&rhs); }

  virtual void Verify() const;
};

}  // namespace bustub
