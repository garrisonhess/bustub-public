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

#include "common/enums/expression_type.h"
#include "common/enums/type_id.h"
#include "nodes/parsenodes.hpp"
#include "parser/group_by_node.h"
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

  //! Transform a Postgres bustub_libpgquery::T_PGIndexStmt node into CreateStatement
  unique_ptr<CreateStatement> TransformCreateIndex(bustub_libpgquery::PGNode *node);

  //! Transform a Postgres bustub_libpgquery::T_PGInsertStmt node into a InsertStatement
  unique_ptr<InsertStatement> TransformInsert(bustub_libpgquery::PGNode *node);

  unique_ptr<DeleteStatement> TransformDelete(bustub_libpgquery::PGNode *node);

  //! Transform a Postgres bustub_libpgquery::T_PGUpdateStmt node into a UpdateStatement
  unique_ptr<UpdateStatement> TransformUpdate(bustub_libpgquery::PGNode *node);

  //===--------------------------------------------------------------------===//
  // Query Node Transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres bustub_libpgquery::T_PGSelectStmt node into a QueryNode
  unique_ptr<QueryNode> TransformSelectNode(bustub_libpgquery::PGSelectStmt *stmt);

  //===--------------------------------------------------------------------===//
  // Expression Transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres boolean expression into an Expression
  unique_ptr<ParsedExpression> TransformBoolExpr(bustub_libpgquery::PGBoolExpr *root);
  //! Transform a Postgres case expression into an Expression
  unique_ptr<ParsedExpression> TransformCase(bustub_libpgquery::PGCaseExpr *root);
  //! Transform a Postgres type cast into an Expression
  unique_ptr<ParsedExpression> TransformTypeCast(bustub_libpgquery::PGTypeCast *root);
  //! Transform a Postgres coalesce into an Expression
  unique_ptr<ParsedExpression> TransformCoalesce(bustub_libpgquery::PGAExpr *root);
  //! Transform a Postgres column reference into an Expression
  unique_ptr<ParsedExpression> TransformColumnRef(bustub_libpgquery::PGColumnRef *root);
  //! Transform a Postgres constant value into an Expression
  unique_ptr<ConstantExpression> TransformValue(bustub_libpgquery::PGValue val);
  //! Transform a Postgres operator into an Expression
  unique_ptr<ParsedExpression> TransformAExpr(bustub_libpgquery::PGAExpr *root);
  //! Transform a Postgres abstract expression into an Expression
  unique_ptr<ParsedExpression> TransformExpression(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres function call into an Expression
  unique_ptr<ParsedExpression> TransformFuncCall(bustub_libpgquery::PGFuncCall *root);
  //! Transform a Postgres boolean expression into an Expression
  unique_ptr<ParsedExpression> TransformInterval(bustub_libpgquery::PGIntervalConstant *root);
  //! Transform a Postgres lambda node [e.g. (x, y) -> x + y] into a lambda expression
  unique_ptr<ParsedExpression> TransformLambda(bustub_libpgquery::PGLambdaFunction *node);
  //! Transform a Postgres array access node (e.g. x[1] or x[1:3])
  unique_ptr<ParsedExpression> TransformArrayAccess(bustub_libpgquery::PGAIndirection *node);
  //! Transform a positional reference (e.g. #1)
  unique_ptr<ParsedExpression> TransformPositionalReference(bustub_libpgquery::PGPositionalReference *node);
  unique_ptr<ParsedExpression> TransformStarExpression(bustub_libpgquery::PGNode *node);

  //! Transform a Postgres constant value into an Expression
  unique_ptr<ParsedExpression> TransformConstant(bustub_libpgquery::PGAConst *c);
  unique_ptr<ParsedExpression> TransformGroupingFunction(bustub_libpgquery::PGGroupingFunc *n);
  unique_ptr<ParsedExpression> TransformResTarget(bustub_libpgquery::PGResTarget *root);
  unique_ptr<ParsedExpression> TransformNullTest(bustub_libpgquery::PGNullTest *root);
  unique_ptr<ParsedExpression> TransformParamRef(bustub_libpgquery::PGParamRef *node);
  unique_ptr<ParsedExpression> TransformNamedArg(bustub_libpgquery::PGNamedArgExpr *root);

  unique_ptr<ParsedExpression> TransformSQLValueFunction(bustub_libpgquery::PGSQLValueFunction *node);

  unique_ptr<ParsedExpression> TransformSubquery(bustub_libpgquery::PGSubLink *root);

  //===--------------------------------------------------------------------===//
  // Helpers
  //===--------------------------------------------------------------------===//
  ColumnDefinition TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef);

  string TransformAlias(bustub_libpgquery::PGAlias *root, vector<string> &column_name_alias);

  //===--------------------------------------------------------------------===//
  // TableRef transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres node into a TableRef
  unique_ptr<TableRef> TransformTableRefNode(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres FROM clause into a TableRef
  unique_ptr<TableRef> TransformFrom(bustub_libpgquery::PGList *root);

  //! Transform a Postgres table reference into a TableRef
  unique_ptr<TableRef> TransformRangeVar(bustub_libpgquery::PGRangeVar *root);
  // //! Transform a Postgres table-producing function into a TableRef
  // unique_ptr<TableRef> TransformRangeFunction(bustub_libpgquery::PGRangeFunction *root);
  // //! Transform a Postgres join node into a TableRef
  // unique_ptr<TableRef> TransformJoin(bustub_libpgquery::PGJoinExpr *root);
  // //! Transform a table producing subquery into a TableRef
  // unique_ptr<TableRef> TransformRangeSubselect(bustub_libpgquery::PGRangeSubselect *root);
  //! Transform a VALUES list into a set of expressions
  unique_ptr<TableRef> TransformValuesList(bustub_libpgquery::PGList *list);

  //! Transform a Postgres TypeName string into a Type
  Type TransformTypeName(bustub_libpgquery::PGTypeName *type_name);

  //! Transform a Postgres GROUP BY expression into a list of Expression
  bool TransformGroupBy(bustub_libpgquery::PGList *group, SelectNode &result);
  // void TransformGroupByNode(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map, SelectNode &result,
  //                           vector<GroupingSet> &result_sets);
  // void AddGroupByExpression(unique_ptr<ParsedExpression> expression, GroupingExpressionMap &map, GroupByNode &result,
  //                           vector<int64_t> &result_set);
  // void TransformGroupByExpression(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map, GroupByNode &result,
  //                                 vector<int64_t> &result_set);

  //! Transform a Postgres ORDER BY expression into an OrderByDescription
  bool TransformOrderBy(bustub_libpgquery::PGList *order, vector<OrderByNode> &result);

  //! Transform a Postgres SELECT clause into a list of Expressions
  void TransformExpressionList(bustub_libpgquery::PGList &list, vector<unique_ptr<ParsedExpression>> &result);
};

}  // namespace bustub
