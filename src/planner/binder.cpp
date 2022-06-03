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

Binder::Binder(bool /*unused*/, ClientContext &context, shared_ptr<Binder> parent_p, bool inherit_ctes_p)
    : context_(context), parent_(move(parent_p)), bound_tables_(0), inherit_ctes_(inherit_ctes_p) {
  parameters_ = nullptr;
  parameter_types_ = nullptr;
}

BoundStatement Binder::Bind(SQLStatement &statement) {
  root_statement_ = &statement;
  LOG_INFO("bind | stmt type %d | stmt query %s | ", static_cast<uint8_t>(statement.type_), statement.query_.c_str());

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
  LOG_INFO("in BindNode for node: %s", node.ToString().c_str());

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

  LOG_INFO("Done with BindNode for node: %s", node.ToString().c_str());
  return result;
}

BoundStatement Binder::Bind(QueryNode &node) {
  LOG_INFO("bind querynode: %s", node.ToString().c_str());
  auto bound_node = BindNode(node);
  BoundStatement result;
  result.names_ = bound_node->names_;
  result.types_ = bound_node->types_;

  // and plan it
  LOG_INFO("Now going to CreatePlan for this node");
  result.plan_ = CreatePlan(*bound_node);
  return result;
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundQueryNode &node) {
  switch (node.type_) {
    case QueryNodeType::SELECT_NODE:
      LOG_INFO("Calling CreatePlan for BoundQueryNode w/ QueryNodeType::SELECT_NODE");
      return CreatePlan((BoundSelectNode &)node);
    case QueryNodeType::SET_OPERATION_NODE:
    case QueryNodeType::RECURSIVE_CTE_NODE:
      throw Exception("Unsupported bound query node type");
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
    case TableReferenceType::JOIN:
      // result = Bind((JoinRef &)ref);
      // break;
      throw Exception("Unknown table ref type");
    default:
      throw Exception("Unknown table ref type");
  }

  LOG_INFO("Binder::Bind(TableRef) Resolved Table Reference: %s", ref.ToString().c_str());
  return result;
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundTableRef &ref) {
  unique_ptr<LogicalOperator> root;
  switch (ref.type_) {
    case TableReferenceType::BASE_TABLE:
      root = CreatePlan((BoundBaseTableRef &)ref);
      break;
    case TableReferenceType::EMPTY:
      root = CreatePlan((BoundEmptyTableRef &)ref);
      break;
    case TableReferenceType::EXPRESSION_LIST:
      root = CreatePlan((BoundExpressionListRef &)ref);
      break;
    case TableReferenceType::JOIN:
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
    if (current->bound_views_.find(view) != current->bound_views_.end()) {
      throw Exception("infinite recursion detected: attempting to recursively bind view");
    }
    current = current->parent_.get();
  }
  bound_views_.insert(view);
}

uint64_t Binder::GenerateTableIndex() {
  if (parent_) {
    return parent_->GenerateTableIndex();
  }
  return bound_tables_++;
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
  if (parent_) {
    return parent_->GetActiveBinders();
  }
  return active_binders_;
}

void Binder::AddUsingBindingSet(unique_ptr<UsingColumnSet> set) {
  if (parent_) {
    parent_->AddUsingBindingSet(move(set));
    return;
  }
  bind_context_.AddUsingBindingSet(move(set));
}

void Binder::SetBindingMode(BindingMode mode) {
  if (parent_) {
    parent_->SetBindingMode(mode);
  }
  this->mode_ = mode;
}

BindingMode Binder::GetBindingMode() {
  if (parent_) {
    return parent_->GetBindingMode();
  }
  return mode_;
}

void Binder::AddTableName(string table_name) {
  if (parent_) {
    parent_->AddTableName(move(table_name));
    return;
  }
  table_names_.insert(move(table_name));
}

const unordered_set<string> &Binder::GetTableNames() {
  if (parent_) {
    return parent_->GetTableNames();
  }
  return table_names_;
}

}  // namespace bustub
