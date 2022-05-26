//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/result_modifier.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>

#include "parser/parsed_expression.h"
#include "type/order_type.h"

namespace bustub {
class FieldWriter;
class FieldReader;

enum ResultModifierType : uint8_t {
  LIMIT_MODIFIER = 1,
  ORDER_MODIFIER = 2,
  DISTINCT_MODIFIER = 3,
  LIMIT_PERCENT_MODIFIER = 4
};

//! A ResultModifier
class ResultModifier {
 public:
  explicit ResultModifier(ResultModifierType type) : type_(type) {}
  virtual ~ResultModifier() = default;

  ResultModifierType type_;

 public:
  //! Returns true if the two result modifiers are equivalent
  virtual bool Equals(const ResultModifier *other) const;

  //! Create a copy of this ResultModifier
  virtual std::unique_ptr<ResultModifier> Copy() const = 0;
  //! Serializes a ResultModifier to a stand-alone binary blob
  void Serialize(Serializer &serializer) const;
  //! Serializes a ResultModifier to a stand-alone binary blob
  virtual void Serialize(FieldWriter &writer) const = 0;
  //! Deserializes a blob back into a ResultModifier
  static std::unique_ptr<ResultModifier> Deserialize(Deserializer &source);
};

//! Single node in ORDER BY statement
struct OrderByNode {
  OrderByNode(OrderType type, OrderByNullType null_order, std::unique_ptr<ParsedExpression> expression)
      : type(type), null_order(null_order), expression_(move(expression)) {}

  //! Sort order, ASC or DESC
  OrderType type_;
  //! The NULL sort order, NULLS_FIRST or NULLS_LAST
  OrderByNullType null_order_;
  //! Expression to order by
  std::unique_ptr<ParsedExpression> expression_;

 public:
  void Serialize(Serializer &serializer) const;
  std::string ToString() const;
  static OrderByNode Deserialize(Deserializer &source);
};

class LimitModifier : public ResultModifier {
 public:
  LimitModifier() : ResultModifier(ResultModifierType::LIMIT_MODIFIER) {}

  //! LIMIT count
  std::unique_ptr<ParsedExpression> limit_;
  //! OFFSET
  std::unique_ptr<ParsedExpression> offset_;

 public:
  bool Equals(const ResultModifier *other) const override;
  std::unique_ptr<ResultModifier> Copy() const override;
  void Serialize(FieldWriter &writer) const override;
  static std::unique_ptr<ResultModifier> Deserialize(FieldReader &reader);
};

class OrderModifier : public ResultModifier {
 public:
  OrderModifier() : ResultModifier(ResultModifierType::ORDER_MODIFIER) {}

  //! List of order nodes
  std::vector<OrderByNode> orders_;

 public:
  bool Equals(const ResultModifier *other) const override;
  std::unique_ptr<ResultModifier> Copy() const override;
  void Serialize(FieldWriter &writer) const override;
  static std::unique_ptr<ResultModifier> Deserialize(FieldReader &reader);
};

class DistinctModifier : public ResultModifier {
 public:
  DistinctModifier() : ResultModifier(ResultModifierType::DISTINCT_MODIFIER) {}

  //! list of distinct on targets (if any)
  std::vector<std::unique_ptr<ParsedExpression>> distinct_on_targets_;

 public:
  bool Equals(const ResultModifier *other) const override;
  std::unique_ptr<ResultModifier> Copy() const override;
  void Serialize(FieldWriter &writer) const override;
  static std::unique_ptr<ResultModifier> Deserialize(FieldReader &reader);
};

class LimitPercentModifier : public ResultModifier {
 public:
  LimitPercentModifier() : ResultModifier(ResultModifierType::LIMIT_PERCENT_MODIFIER) {}

  //! LIMIT %
  std::unique_ptr<ParsedExpression> limit_;
  //! OFFSET
  std::unique_ptr<ParsedExpression> offset_;

 public:
  bool Equals(const ResultModifier *other) const override;
  std::unique_ptr<ResultModifier> Copy() const override;
  void Serialize(FieldWriter &writer) const override;
  static std::unique_ptr<ResultModifier> Deserialize(FieldReader &reader);
};

}  // namespace bustub
