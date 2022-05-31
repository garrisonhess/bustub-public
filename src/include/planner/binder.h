//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/binder.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>

#include "common/case_insensitive_map.h"
#include "common/enums/statement_type.h"
#include "parser/column_definition.h"
#include "parser/result_modifier.h"
#include "parser/tokens.h"
#include "planner/bind_context.h"
#include "planner/bound_statement.h"
#include "planner/bound_tokens.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/logical_operator.h"

namespace bustub {
class BoundResultModifier;
class BoundSelectNode;
class ClientContext;
class ExpressionBinder;
class LimitModifier;
class OrderBinder;
class TableCatalogEntry;
class ViewCatalogEntry;
class TableMacroCatalogEntry;

struct CreateInfo;
struct BoundCreateTableInfo;

enum class BindingMode : uint8_t { STANDARD_BINDING, EXTRACT_NAMES };

//! Bind the parsed query tree to the actual columns present in the catalog.
/*!
  The binder is responsible for binding tables and columns to actual physical
  tables and columns in the catalog. In the process, it also resolves types of
  all expressions.
*/
class Binder : public std::enable_shared_from_this<Binder> {
  friend class ExpressionBinder;
  friend class SelectBinder;
  friend class RecursiveSubqueryPlanner;

 public:
  static shared_ptr<Binder> CreateBinder(ClientContext &context, Binder *parent = nullptr, bool inherit_ctes = true);

  //! The client context
  ClientContext &context;
  //! The bind context
  BindContext bind_context;
  //! The set of parameter expressions bound by this binder
  vector<BoundParameterExpression *> *parameters;
  //! The types of the prepared statement parameters, if any
  vector<Type> *parameter_types;
  //! The alias for the currently processing subquery, if it exists
  string alias;

 public:
  BoundStatement Bind(SQLStatement &statement);
  BoundStatement Bind(QueryNode &node);

  // SchemaCatalogEntry *BindSchema(CreateInfo &info);
  // SchemaCatalogEntry *BindCreateFunctionInfo(CreateInfo &info);

  unique_ptr<BoundTableRef> Bind(TableRef &ref);
  unique_ptr<LogicalOperator> CreatePlan(BoundTableRef &ref);

  //! Generates an unused index for a table
  uint64_t GenerateTableIndex();

  //! Add the view to the set of currently bound views - used for detecting recursive view definitions
  void AddBoundView(ViewCatalogEntry *view);

  void PushExpressionBinder(ExpressionBinder *binder);
  void PopExpressionBinder();
  void SetActiveBinder(ExpressionBinder *binder);
  ExpressionBinder *GetActiveBinder();
  bool HasActiveBinder();

  vector<ExpressionBinder *> &GetActiveBinders();

  string FormatError(ParsedExpression &expr_context, const string &message);
  string FormatError(TableRef &ref_context, const string &message);

  static void BindType(ClientContext &context, Type &type, const string &schema = "");

  bool HasMatchingBinding(const string &table_name, const string &column_name, string &error_message);
  bool HasMatchingBinding(const string &schema_name, const string &table_name, const string &column_name,
                          string &error_message);

  void SetBindingMode(BindingMode mode);
  BindingMode GetBindingMode();
  void AddTableName(string table_name);
  const unordered_set<string> &GetTableNames();

 private:
  //! The parent binder (if any)
  shared_ptr<Binder> parent;
  //! The vector of active binders
  vector<ExpressionBinder *> active_binders;
  //! The count of bound_tables
  uint64_t bound_tables;
  //! Whether or not the binder has any unplanned subqueries that still need to be planned
  bool has_unplanned_subqueries = false;
  //! Whether or not subqueries should be planned already
  bool plan_subquery = true;
  //! Whether CTEs should reference the parent binder (if it exists)
  bool inherit_ctes = true;
  //! Whether or not the binder can contain NULLs as the root of expressions
  bool can_contain_nulls = false;
  //! The root statement of the query that is currently being parsed
  SQLStatement *root_statement = nullptr;
  //! Binding mode
  BindingMode mode = BindingMode::STANDARD_BINDING;
  //! Table names extracted for BindingMode::EXTRACT_NAMES
  unordered_set<string> table_names;
  //! The set of bound views
  unordered_set<ViewCatalogEntry *> bound_views;

 private:
  //! Bind the default values of the columns of a table
  void BindDefaultValues(vector<ColumnDefinition> &columns, vector<unique_ptr<Expression>> &bound_defaults);
  //! Bind a limit value (LIMIT or OFFSET)
  unique_ptr<Expression> BindDelimiter(ClientContext &context, OrderBinder &order_binder,
                                       unique_ptr<ParsedExpression> delimiter, const Type &type,
                                       Value &delimiter_value);

  //! Move correlated expressions from the child binder to this binder
  void MoveCorrelatedExpressions(Binder &other);

  BoundStatement Bind(SelectStatement &stmt);
  // BoundStatement Bind(InsertStatement &stmt);
  // BoundStatement Bind(DeleteStatement &stmt);
  // BoundStatement Bind(UpdateStatement &stmt);
  // BoundStatement Bind(CreateStatement &stmt);

  unique_ptr<BoundQueryNode> BindNode(SelectNode &node);
  unique_ptr<BoundQueryNode> BindNode(QueryNode &node);

  unique_ptr<LogicalOperator> VisitQueryNode(BoundQueryNode &node, unique_ptr<LogicalOperator> root);
  unique_ptr<LogicalOperator> CreatePlan(BoundSelectNode &statement);
  unique_ptr<LogicalOperator> CreatePlan(BoundQueryNode &node);

  unique_ptr<BoundTableRef> Bind(BaseTableRef &ref);
  unique_ptr<BoundTableRef> Bind(JoinRef &ref);
  unique_ptr<BoundTableRef> Bind(EmptyTableRef &ref);
  unique_ptr<BoundTableRef> Bind(ExpressionListRef &ref);

  unique_ptr<LogicalOperator> CreatePlan(BoundBaseTableRef &ref);
  // unique_ptr<LogicalOperator> CreatePlan(BoundJoinRef &ref);
  unique_ptr<LogicalOperator> CreatePlan(BoundEmptyTableRef &ref);
  unique_ptr<LogicalOperator> CreatePlan(BoundExpressionListRef &ref);

  void BindModifiers(OrderBinder &order_binder, QueryNode &statement, BoundQueryNode &result);
  void BindModifierTypes(BoundQueryNode &result, const vector<Type> &sql_types, uint64_t projection_index);

  // unique_ptr<BoundResultModifier> BindLimit(OrderBinder &order_binder, LimitModifier &limit_mod);
  // unique_ptr<Expression> BindOrderExpression(OrderBinder &order_binder, unique_ptr<ParsedExpression> expr);

  unique_ptr<LogicalOperator> PlanFilter(unique_ptr<Expression> condition, unique_ptr<LogicalOperator> root);

  unique_ptr<LogicalOperator> CastLogicalOperatorToTypes(vector<Type> &source_types, vector<Type> &target_types,
                                                         unique_ptr<LogicalOperator> op);

  string FindBinding(const string &using_column, const string &join_side);
  bool TryFindBinding(const string &using_column, const string &join_side, string &result);

  void AddUsingBindingSet(unique_ptr<UsingColumnSet> set);
  string RetrieveUsingBinding(Binder &current_binder, UsingColumnSet *current_set, const string &column_name,
                              const string &join_side, UsingColumnSet *new_set);

 public:
  // This should really be a private constructor, but make_shared does not allow it...
  // If you are thinking about calling this, you should probably call Binder::CreateBinder
  Binder(bool I_know_what_I_am_doing, ClientContext &context, shared_ptr<Binder> parent, bool inherit_ctes);
};

}  // namespace bustub
