//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/transformer.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "nodes/parsenodes.hpp"
#include "parser/group_by_node.h"
#include "parser/parsed_data/create_info.h"
#include "parser/qualified_name.h"
#include "parser/tokens.h"
#include "pg_definitions.hpp"
#include "type/expression_type.h"
#include "type/type_id.h"

namespace bustub {
using std::string;
using std::unique_ptr;
using std::vector;

class ColumnDefinition;
class StackChecker;
struct OrderByNode;
struct CopyInfo;
struct CommonTableExpressionInfo;
struct GroupingExpressionMap;

//! The transformer class is responsible for transforming the internal Postgres
//! parser representation into the DuckDB representation
class Transformer {
  friend class StackChecker;

 public:
  explicit Transformer(int64_t max_expression_depth_p);
  explicit Transformer(Transformer *parent);

  //! Transforms a Postgres parse tree into a set of SQL Statements
  bool TransformParseTree(bustub_libpgquery::PGList *tree, std::vector<std::unique_ptr<SQLStatement>> &statements);
  std::string NodetypeToString(bustub_libpgquery::PGNodeTag type);

  int64_t ParamCount() { return parent_ != nullptr ? parent_->ParamCount() : prepared_statement_parameter_index_; }

 private:
  Transformer *parent_;
  int64_t max_expression_depth_;
  //! The current prepared statement parameter index
  int64_t prepared_statement_parameter_index_ = 0;
  //! Holds window expressions defined by name. We need those when transforming the expressions referring to them.
  std::unordered_map<std::string, bustub_libpgquery::PGWindowDef *> window_clauses_;

  void SetParamCount(int64_t new_count) {
    if (parent_ != nullptr) {
      parent_->SetParamCount(new_count);
    } else {
      this->prepared_statement_parameter_index_ = new_count;
    }
  }

 private:
  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatement(bustub_libpgquery::PGNode *stmt);
  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatementInternal(bustub_libpgquery::PGNode *stmt);
  //===--------------------------------------------------------------------===//
  // Statement transformation
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres bustub_libpgquery::T_PGSelectStmt node into a SelectStatement
  unique_ptr<SelectStatement> TransformSelect(bustub_libpgquery::PGNode *node, bool isSelect = true);
  //! Transform a Postgres T_AlterStmt node into a AlterStatement
  unique_ptr<AlterStatement> TransformAlter(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGRenameStmt node into a RenameStatement
  unique_ptr<AlterStatement> TransformRename(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCreateStmt node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateTable(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCreateStmt node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateTableAs(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateSchema(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCreateSeqStmt node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateSequence(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGViewStmt node into a CreateStatement
  unique_ptr<CreateStatement> TransformCreateView(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGIndexStmt node into CreateStatement
  unique_ptr<CreateStatement> TransformCreateIndex(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCreateFunctionStmt node into CreateStatement
  unique_ptr<CreateStatement> TransformCreateFunction(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCreateEnumStmt node into CreateStatement
  unique_ptr<CreateStatement> TransformCreateEnum(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGAlterSeqStmt node into CreateStatement
  unique_ptr<AlterStatement> TransformAlterSequence(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGDropStmt node into a Drop[Table,Schema]Statement
  unique_ptr<SQLStatement> TransformDrop(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGInsertStmt node into a InsertStatement
  unique_ptr<InsertStatement> TransformInsert(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGCopyStmt node into a CopyStatement
  unique_ptr<CopyStatement> TransformCopy(bustub_libpgquery::PGNode *node);
  void TransformCopyOptions(CopyInfo &info, bustub_libpgquery::PGList *options);
  //! Transform a Postgres bustub_libpgquery::T_PGTransactionStmt node into a TransactionStatement
  unique_ptr<TransactionStatement> TransformTransaction(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres T_DeleteStatement node into a DeleteStatement
  unique_ptr<DeleteStatement> TransformDelete(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGUpdateStmt node into a UpdateStatement
  unique_ptr<UpdateStatement> TransformUpdate(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGPragmaStmt node into a PragmaStatement
  unique_ptr<SQLStatement> TransformPragma(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGExportStmt node into a ExportStatement
  unique_ptr<ExportStatement> TransformExport(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres bustub_libpgquery::T_PGImportStmt node into a PragmaStatement
  unique_ptr<PragmaStatement> TransformImport(bustub_libpgquery::PGNode *node);
  unique_ptr<ExplainStatement> TransformExplain(bustub_libpgquery::PGNode *node);
  unique_ptr<VacuumStatement> TransformVacuum(bustub_libpgquery::PGNode *node);
  unique_ptr<SQLStatement> TransformShow(bustub_libpgquery::PGNode *node);
  unique_ptr<ShowStatement> TransformShowSelect(bustub_libpgquery::PGNode *node);

  unique_ptr<PrepareStatement> TransformPrepare(bustub_libpgquery::PGNode *node);
  unique_ptr<ExecuteStatement> TransformExecute(bustub_libpgquery::PGNode *node);
  unique_ptr<CallStatement> TransformCall(bustub_libpgquery::PGNode *node);
  unique_ptr<DropStatement> TransformDeallocate(bustub_libpgquery::PGNode *node);
  unique_ptr<SetStatement> TransformSet(bustub_libpgquery::PGNode *node);
  unique_ptr<SQLStatement> TransformCheckpoint(bustub_libpgquery::PGNode *node);
  unique_ptr<LoadStatement> TransformLoad(bustub_libpgquery::PGNode *node);

  //===--------------------------------------------------------------------===//
  // Query Node Transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres bustub_libpgquery::T_PGSelectStmt node into a QueryNode
  unique_ptr<QueryNode> TransformSelectNode(bustub_libpgquery::PGSelectStmt *node);

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
  // Constraints transform
  //===--------------------------------------------------------------------===//
  unique_ptr<Constraint> TransformConstraint(bustub_libpgquery::PGListCell *cell);

  unique_ptr<Constraint> TransformConstraint(bustub_libpgquery::PGListCell *cell, ColumnDefinition &column,
                                             int64_t index);

  //===--------------------------------------------------------------------===//
  // Collation transform
  //===--------------------------------------------------------------------===//
  unique_ptr<ParsedExpression> TransformCollateExpr(bustub_libpgquery::PGCollateClause *collate);

  string TransformCollation(bustub_libpgquery::PGCollateClause *collate);

  ColumnDefinition TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef);
  //===--------------------------------------------------------------------===//
  // Helpers
  //===--------------------------------------------------------------------===//
  // OnCreateConflict TransformOnConflict(bustub_libpgquery::PGOnCreateConflict conflict);
  string TransformAlias(bustub_libpgquery::PGAlias *root, std::vector<string> &column_name_alias);
  void TransformCTE(bustub_libpgquery::PGWithClause *de_with_clause, QueryNode &select);
  unique_ptr<SelectStatement> TransformRecursiveCTE(bustub_libpgquery::PGCommonTableExpr *node,
                                                    CommonTableExpressionInfo &info);
  // Operator String to ExpressionType (e.g. + => OPERATOR_ADD)
  ExpressionType OperatorToExpressionType(const string &op);

  unique_ptr<ParsedExpression> TransformUnaryOperator(const string &op, unique_ptr<ParsedExpression> child);
  unique_ptr<ParsedExpression> TransformBinaryOperator(const string &op, unique_ptr<ParsedExpression> left,
                                                       unique_ptr<ParsedExpression> right);
  //===--------------------------------------------------------------------===//
  // TableRef transform
  //===--------------------------------------------------------------------===//
  //! Transform a Postgres node into a TableRef
  unique_ptr<TableRef> TransformTableRefNode(bustub_libpgquery::PGNode *node);
  //! Transform a Postgres FROM clause into a TableRef
  unique_ptr<TableRef> TransformFrom(bustub_libpgquery::PGList *root);
  //! Transform a Postgres table reference into a TableRef
  unique_ptr<TableRef> TransformRangeVar(bustub_libpgquery::PGRangeVar *root);
  //! Transform a Postgres table-producing function into a TableRef
  unique_ptr<TableRef> TransformRangeFunction(bustub_libpgquery::PGRangeFunction *root);
  //! Transform a Postgres join node into a TableRef
  unique_ptr<TableRef> TransformJoin(bustub_libpgquery::PGJoinExpr *root);
  //! Transform a table producing subquery into a TableRef
  unique_ptr<TableRef> TransformRangeSubselect(bustub_libpgquery::PGRangeSubselect *root);
  //! Transform a VALUES list into a set of expressions
  unique_ptr<TableRef> TransformValuesList(bustub_libpgquery::PGList *list);

  // //! Transform a range var into a (schema) qualified name
  // QualifiedName TransformQualifiedName(bustub_libpgquery::PGRangeVar *root);

  //! Transform a Postgres TypeName string into a LogicalType
  TypeId TransformTypeName(bustub_libpgquery::PGTypeName *name);

  //! Transform a Postgres GROUP BY expression into a list of Expression
  bool TransformGroupBy(bustub_libpgquery::PGList *group, SelectNode &result);
  void TransformGroupByNode(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map, SelectNode &result,
                            vector<GroupingSet> &result_sets);
  void AddGroupByExpression(unique_ptr<ParsedExpression> expression, GroupingExpressionMap &map, GroupByNode &result,
                            vector<int64_t> &result_set);
  void TransformGroupByExpression(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map, GroupByNode &result,
                                  vector<int64_t> &result_set);
  //! Transform a Postgres ORDER BY expression into an OrderByDescription
  bool TransformOrderBy(bustub_libpgquery::PGList *order, vector<OrderByNode> &result);

  //! Transform a Postgres SELECT clause into a list of Expressions
  void TransformExpressionList(bustub_libpgquery::PGList &list, vector<unique_ptr<ParsedExpression>> &result);

  //! Transform a Postgres PARTITION BY/ORDER BY specification into lists of expressions
  void TransformWindowDef(bustub_libpgquery::PGWindowDef *window_spec, WindowExpression *expr);
  //! Transform a Postgres window frame specification into frame expressions
  void TransformWindowFrame(bustub_libpgquery::PGWindowDef *window_spec, WindowExpression *expr);

  unique_ptr<SampleOptions> TransformSampleOptions(bustub_libpgquery::PGNode *options);

 private:
  //! Current stack depth
  int64_t stack_depth_;

  void InitializeStackCheck();
  StackChecker StackCheck(int64_t extra_stack = 1);
};

class StackChecker {
 public:
  StackChecker(Transformer &transformer, int64_t stack_usage);
  ~StackChecker();
  StackChecker(StackChecker &&) noexcept;
  StackChecker(const StackChecker &) = delete;

 private:
  Transformer &transformer_;
  int64_t stack_usage_;
};

}  // namespace bustub
