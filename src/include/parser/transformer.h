//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/transformer.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "catalog/column.h"
#include "common/enums/expression_type.h"
#include "common/enums/type_id.h"
#include "nodes/parsenodes.hpp"
#include "parser/tokens.h"
#include "pg_definitions.hpp"

namespace bustub {
class ColumnDefinition;
struct OrderByNode;

//! The transformer class is responsible for transforming the internal Postgres
//! parser representation into the BusTub representation
class Transformer {
 public:
  explicit Transformer(Transformer *parent);

  //! Transforms a Postgres parse tree into a set of SQL Statements
  bool TransformParseTree(bustub_libpgquery::PGList *tree, std::vector<std::unique_ptr<SQLStatement>> &statements);

  std::string NodetypeToString(bustub_libpgquery::PGNodeTag type);

 private:
  Transformer *parent_;

 private:
  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatement(bustub_libpgquery::PGNode *stmt);

  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatementInternal(bustub_libpgquery::PGNode *stmt);

  //===--------------------------------------------------------------------===//
  // Statement transformation
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres bustub_libpgquery::T_PGSelectStmt node into a SelectStatement
  unique_ptr<SelectStatement> TransformSelect(bustub_libpgquery::PGNode *node, bool is_select = true);

  //! Transform a Postgres bustub_libpgquery::T_PGCreateStmt node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateTable(bustub_libpgquery::PGNode *node);

  // //! Transform a Postgres bustub_libpgquery::T_PGIndexStmt node into CreateStatement
  // unique_ptr<CreateStatement> TransformCreateIndex(bustub_libpgquery::PGNode *node);

  // //! Transform a Postgres bustub_libpgquery::T_PGInsertStmt node into a InsertStatement
  // unique_ptr<InsertStatement> TransformInsert(bustub_libpgquery::PGNode *node);

  // unique_ptr<DeleteStatement> TransformDelete(bustub_libpgquery::PGNode *node);

  // //! Transform a Postgres bustub_libpgquery::T_PGUpdateStmt node into a UpdateStatement
  // unique_ptr<UpdateStatement> TransformUpdate(bustub_libpgquery::PGNode *node);

  //===--------------------------------------------------------------------===//
  // Query Node Transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres bustub_libpgquery::T_PGSelectStmt node into a QueryNode
  unique_ptr<QueryNode> TransformSelectNode(bustub_libpgquery::PGSelectStmt *stmt);

  //===--------------------------------------------------------------------===//
  // Expression Transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres column reference into an Expression
  unique_ptr<AbstractExpression> TransformColumnRef(bustub_libpgquery::PGColumnRef *root);

  //! Transform a Postgres constant value into an Expression
  unique_ptr<ConstantExpression> TransformValue(bustub_libpgquery::PGValue val);

  //! Transform a Postgres constant value into an Expression
  unique_ptr<AbstractExpression> TransformConstant(bustub_libpgquery::PGAConst *c);

  Column TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef);

  //===--------------------------------------------------------------------===//
  // TableRef transform
  //===--------------------------------------------------------------------===//
  // //! Transform a Postgres node into a TableRef
  // unique_ptr<TableRef> TransformTableRefNode(bustub_libpgquery::PGNode *node);

  // //! Transform a Postgres FROM clause into a TableRef
  // unique_ptr<TableRef> TransformFrom(bustub_libpgquery::PGList *root);

  // //! Transform a Postgres table reference into a TableRef
  // unique_ptr<TableRef> TransformRangeVar(bustub_libpgquery::PGRangeVar *root);

  // //! Transform a VALUES list into a set of expressions
  // unique_ptr<TableRef> TransformValuesList(bustub_libpgquery::PGList *list);

  // //! Transform a Postgres TypeName string into a Type
  // Type TransformTypeName(bustub_libpgquery::PGTypeName *type_name);

  // //! Transform a Postgres SELECT clause into a list of Expressions
  // void TransformExpressionList(bustub_libpgquery::PGList &list, vector<unique_ptr<AbstractExpression>> &result);
};

}  // namespace bustub
