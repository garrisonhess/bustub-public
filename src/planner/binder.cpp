#include "planner/binder.h"

#include "parser/query_node.h"
#include "parser/query_node/select_node.h"
#include "parser/statement/list.h"
#include "planner/bound_query_node.h"
#include "planner/bound_tableref.h"
#include "planner/expression.h"
#include "planner/operator/logical_projection.h"

#include <algorithm>

namespace bustub {

shared_ptr<Binder> Binder::CreateBinder(ClientContext &context, Binder *parent, bool inherit_ctes) {
  return make_shared<Binder>(true, context, parent != nullptr ? parent->shared_from_this() : nullptr, inherit_ctes);
}

Binder::Binder(bool, ClientContext &context, shared_ptr<Binder> parent_p, bool inherit_ctes_p)
    : context(context), parent(move(parent_p)), bound_tables(0), inherit_ctes(inherit_ctes_p) {
  parameters = nullptr;
  parameter_types = nullptr;
}

BoundStatement Binder::Bind(SQLStatement &statement) {
  root_statement = &statement;
  switch (statement.type_) {
    case StatementType::SELECT_STATEMENT:
      return Bind((SelectStatement &)statement);
    case StatementType::INSERT_STATEMENT:
      return Bind((InsertStatement &)statement);
    case StatementType::DELETE_STATEMENT:
      return Bind((DeleteStatement &)statement);
    case StatementType::UPDATE_STATEMENT:
      return Bind((UpdateStatement &)statement);
    case StatementType::CREATE_STATEMENT:
      return Bind((CreateStatement &)statement);
    case StatementType::DROP_STATEMENT:
    case StatementType::ALTER_STATEMENT:
    case StatementType::TRANSACTION_STATEMENT:
    case StatementType::PRAGMA_STATEMENT:
    case StatementType::EXPLAIN_STATEMENT:
    case StatementType::VACUUM_STATEMENT:
    case StatementType::SHOW_STATEMENT:
    case StatementType::RELATION_STATEMENT:
    case StatementType::EXPORT_STATEMENT:
    case StatementType::SET_STATEMENT:
    case StatementType::LOAD_STATEMENT:
    case StatementType::COPY_STATEMENT:
      throw NotImplementedException("Unimplemented statement type for Bind");
    default:
      throw NotImplementedException("Unimplemented statement type for Bind");
  }
}

unique_ptr<BoundQueryNode> Binder::BindNode(QueryNode &node) {
  // now we bind the node
  unique_ptr<BoundQueryNode> result;
  switch (node.type_) {
    case QueryNodeType::SELECT_NODE:
      result = BindNode((SelectNode &)node);
      break;
    case QueryNodeType::RECURSIVE_CTE_NODE:
    case QueryNodeType::SET_OPERATION_NODE:
    case QueryNodeType::BOUND_SUBQUERY_NODE:
      throw NotImplementedException("Unimplemented node type for Bind");
    default:
      throw NotImplementedException("Unimplemented node type for Bind");
  }
  return result;
}

BoundStatement Binder::Bind(QueryNode &node) {
  auto bound_node = BindNode(node);

  BoundStatement result;
  result.names = bound_node->names;
  result.types = bound_node->types;

  // and plan it
  result.plan = CreatePlan(*bound_node);
  return result;
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundQueryNode &node) {
  switch (node.type) {
    case QueryNodeType::SELECT_NODE:
      return CreatePlan((BoundSelectNode &)node);
    case QueryNodeType::SET_OPERATION_NODE:
      return CreatePlan((BoundSetOperationNode &)node);
    case QueryNodeType::RECURSIVE_CTE_NODE:
      return CreatePlan((BoundRecursiveCTENode &)node);
    default:
      throw Exception("Unsupported bound query node type");
  }
}

unique_ptr<BoundTableRef> Binder::Bind(TableRef &ref) {
  unique_ptr<BoundTableRef> result;
  switch (ref.type_) {
    case TableReferenceType::BASE_TABLE:
      result = Bind((BaseTableRef &)ref);
      break;
    case TableReferenceType::JOIN:
      result = Bind((JoinRef &)ref);
      break;
    case TableReferenceType::EMPTY:
      result = Bind((EmptyTableRef &)ref);
      break;
    case TableReferenceType::EXPRESSION_LIST:
      result = Bind((ExpressionListRef &)ref);
      break;
    case TableReferenceType::CROSS_PRODUCT:
    case TableReferenceType::SUBQUERY:
    case TableReferenceType::TABLE_FUNCTION:
      break;
    default:
      throw Exception("Unknown table ref type");
  }
  return result;
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundTableRef &ref) {
  unique_ptr<LogicalOperator> root;
  switch (ref.type) {
    case TableReferenceType::BASE_TABLE:
      root = CreatePlan((BoundBaseTableRef &)ref);
      break;
    case TableReferenceType::JOIN:
      root = CreatePlan((BoundJoinRef &)ref);
      break;
    case TableReferenceType::EMPTY:
      root = CreatePlan((BoundEmptyTableRef &)ref);
      break;
    case TableReferenceType::EXPRESSION_LIST:
      root = CreatePlan((BoundExpressionListRef &)ref);
      break;
    case TableReferenceType::TABLE_FUNCTION:
    case TableReferenceType::SUBQUERY:
    case TableReferenceType::CROSS_PRODUCT:
    case TableReferenceType::CTE:
      throw Exception("Unsupported bound table ref type type");
    default:
      throw Exception("Unsupported bound table ref type type");
  }
  return root;
}

void Binder::AddBoundView(ViewCatalogEntry *view) {
  // check if the view is already bound
  auto current = this;
  while (current != nullptr) {
    if (current->bound_views.find(view) != current->bound_views.end()) {
      throw Exception("infinite recursion detected: attempting to recursively bind view");
    }
    current = current->parent.get();
  }
  bound_views.insert(view);
}

uint64_t Binder::GenerateTableIndex() {
  if (parent) {
    return parent->GenerateTableIndex();
  }
  return bound_tables++;
}

void Binder::PushExpressionBinder(ExpressionBinder *binder) { GetActiveBinders().push_back(binder); }

void Binder::PopExpressionBinder() {
  assert(HasActiveBinder());
  GetActiveBinders().pop_back();
}

void Binder::SetActiveBinder(ExpressionBinder *binder) {
  assert(HasActiveBinder());
  GetActiveBinders().back() = binder;
}

ExpressionBinder *Binder::GetActiveBinder() { return GetActiveBinders().back(); }

bool Binder::HasActiveBinder() { return !GetActiveBinders().empty(); }

vector<ExpressionBinder *> &Binder::GetActiveBinders() {
  if (parent) {
    return parent->GetActiveBinders();
  }
  return active_binders;
}

void Binder::AddUsingBindingSet(unique_ptr<UsingColumnSet> set) {
  if (parent) {
    parent->AddUsingBindingSet(move(set));
    return;
  }
  bind_context.AddUsingBindingSet(move(set));
}

bool Binder::HasMatchingBinding(const string &table_name, const string &column_name, string &error_message) {
  string empty_schema;
  return HasMatchingBinding(empty_schema, table_name, column_name, error_message);
}

bool Binder::HasMatchingBinding(const string &schema_name, const string &table_name, const string &column_name,
                                string &error_message) {
  Binding *binding = bind_context.GetBinding(table_name, error_message);
  if (binding == nullptr) {
    return false;
  }
  if (!schema_name.empty()) {
    auto table_entry = binding->GetTableEntry();
    if (table_entry == nullptr) {
      return false;
    }
    if (table_entry->schema->name != schema_name || table_entry->name != table_name) {
      return false;
    }
  }
  if (!binding->HasMatchingBinding(column_name)) {
    error_message = binding->ColumnNotFoundError(column_name);
    return false;
  }
  return true;
}

void Binder::SetBindingMode(BindingMode mode) {
  if (parent) {
    parent->SetBindingMode(mode);
  }
  this->mode = mode;
}

BindingMode Binder::GetBindingMode() {
  if (parent) {
    return parent->GetBindingMode();
  }
  return mode;
}

void Binder::AddTableName(string table_name) {
  if (parent) {
    parent->AddTableName(move(table_name));
    return;
  }
  table_names.insert(move(table_name));
}

const unordered_set<string> &Binder::GetTableNames() {
  if (parent) {
    return parent->GetTableNames();
  }
  return table_names;
}

}  // namespace bustub
