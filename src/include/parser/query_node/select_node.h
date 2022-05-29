//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/query_node/select_node.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include "parser/group_by_node.h"
#include "parser/parsed_expression.h"
#include "parser/query_node.h"
#include "parser/sql_statement.h"
#include "parser/tableref.h"

namespace bustub {

using std::unique_ptr;

//! SelectNode represents a standard SELECT statement
class SelectNode : public QueryNode {
 public:
  SelectNode();

  //! The projection list
  vector<unique_ptr<ParsedExpression>> select_list_;
  //! The FROM clause
  unique_ptr<TableRef> from_table_;
  //! The WHERE clause
  unique_ptr<ParsedExpression> where_clause_;
  //! list of groups
  GroupByNode groups_;
  //! HAVING clause
  unique_ptr<ParsedExpression> having_;
  //! QUALIFY clause
  unique_ptr<ParsedExpression> qualify_;

  const vector<unique_ptr<ParsedExpression>> &GetSelectList() const override { return select_list_; }

 public:
  //! Convert the query node to a string
  string ToString() const override;

  bool Equals(const QueryNode *other) const override;
  //! Create a copy of this SelectNode
  unique_ptr<QueryNode> Copy() const override;

  //! Serializes a QueryNode to a stand-alone binary blob
  void Serialize(FieldWriter &writer) const override;
  //! Deserializes a blob back into a QueryNode
  static unique_ptr<QueryNode> Deserialize(FieldReader &reader);
};

}  // namespace bustub
