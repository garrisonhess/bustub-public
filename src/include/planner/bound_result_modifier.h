//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/bound_result_modifier.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/limits.h"
#include "parser/result_modifier.h"
#include "planner/bound_statement.h"
#include "planner/expression.h"

namespace bustub {

//! A ResultModifier
class BoundResultModifier {
 public:
  explicit BoundResultModifier(ResultModifierType type);
  virtual ~BoundResultModifier();

  ResultModifierType type_;
};

struct BoundOrderByNode {
 public:
  BoundOrderByNode(OrderType type, OrderByNullType null_order, unique_ptr<Expression> expression);

  OrderType type;
  OrderByNullType null_order;
  unique_ptr<Expression> expression;

 public:
  BoundOrderByNode Copy() const;
  string ToString() const;
};

class BoundLimitModifier : public BoundResultModifier {
 public:
  BoundLimitModifier();

  //! LIMIT
  int64_t limit_val = BUSTUB_INT64_MAX;
  //! OFFSET
  int64_t offset_val = 0;
  //! Expression in case limit is not constant
  unique_ptr<Expression> limit;
  //! Expression in case limit is not constant
  unique_ptr<Expression> offset;
};

class BoundOrderModifier : public BoundResultModifier {
 public:
  BoundOrderModifier();

  //! List of order nodes
  vector<BoundOrderByNode> orders;
};

class BoundDistinctModifier : public BoundResultModifier {
 public:
  BoundDistinctModifier();

  //! list of distinct on targets (if any)
  vector<unique_ptr<Expression>> target_distincts;
};

class BoundLimitPercentModifier : public BoundResultModifier {
 public:
  BoundLimitPercentModifier();

  //! LIMIT %
  double limit_percent = 100.0;
  //! OFFSET
  int64_t offset_val = 0;
  //! Expression in case limit is not constant
  unique_ptr<Expression> limit;
  //! Expression in case limit is not constant
  unique_ptr<Expression> offset;
};

}  // namespace bustub