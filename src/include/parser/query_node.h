//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/query_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "parser/result_modifier.h"

#include <cstdint>
#include <string>
#include <vector>

namespace bustub {
using std::unique_ptr;
using std::vector;

enum QueryNodeType : uint8_t {
  SELECT_NODE = 1,
  SET_OPERATION_NODE = 2,
  BOUND_SUBQUERY_NODE = 3,
  RECURSIVE_CTE_NODE = 4
};

class QueryNode {
 public:
  explicit QueryNode(QueryNodeType type) : type_(type) {}
  virtual ~QueryNode() = default;

  //! The type of the query node, either SetOperation or Select
  QueryNodeType type_;

  //! The set of result modifiers associated with this query node
  vector<unique_ptr<ResultModifier>> modifiers_;

  //   //! CTEs (used by SelectNode and SetOperationNode)
  //   unordered_map<string, unique_ptr<CommonTableExpressionInfo>> cte_map_;

  virtual const vector<unique_ptr<ParsedExpression>> &GetSelectList() const = 0;

 public:
  //! Convert the query node to a string
  virtual std::string ToString() const = 0;

  virtual bool Equals(const QueryNode *other) const;

  //! Create a copy of this QueryNode
  virtual std::unique_ptr<QueryNode> Copy() const = 0;

  //! Serializes a QueryNode to a stand-alone binary blob
  void Serialize(Serializer &serializer) const;

  //! Serializes a QueryNode to a stand-alone binary blob
  virtual void Serialize(FieldWriter &writer) const = 0;

  //! Deserializes a blob back into a QueryNode
  static std::unique_ptr<QueryNode> Deserialize(Deserializer &source);

  // std::string CTEToString() const;
  std::string ResultModifiersToString() const;

 protected:
  //! Copy base QueryNode properties from another expression to this one,
  //! used in Copy method
  void CopyProperties(QueryNode &other) const;
};

}  // namespace bustub
