#include "parser/statement/insert_statement.h"
#include "common/constants.h"
#include "parser/keyword_helper.h"
#include "parser/query_node/select_node.h"
#include "parser/tableref/expressionlistref.h"

namespace bustub {

InsertStatement::InsertStatement() : SQLStatement(StatementType::INSERT_STATEMENT), schema_(DEFAULT_SCHEMA) {}

InsertStatement::InsertStatement(const InsertStatement &other)
    : SQLStatement(other),
      select_statement_(
          unique_ptr<SelectStatement>(static_cast<SelectStatement *>((other.select_statement_->Copy().release())))),
      columns_(other.columns_),
      table_(other.table_),
      schema_(other.schema_) {}

string InsertStatement::ToString() const {
  string result;
  result = "INSERT INTO ";
  if (!schema_.empty()) {
    result += KeywordHelper::WriteOptionallyQuoted(schema_) + ".";
  }
  result += KeywordHelper::WriteOptionallyQuoted(table_);
  if (!columns_.empty()) {
    result += " (";
    for (uint64_t i = 0; i < columns_.size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += KeywordHelper::WriteOptionallyQuoted(columns_[i]);
    }
    result += " )";
  }
  result += " ";
  auto values_list = GetValuesList();
  if (values_list != nullptr) {
    values_list->alias_ = string();
    result += values_list->ToString();
  } else {
    result += select_statement_->ToString();
  }
  if (!returning_list_.empty()) {
    result += " RETURNING ";
    for (uint64_t i = 0; i < returning_list_.size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += returning_list_[i]->ToString();
    }
  }
  return result;
}

unique_ptr<SQLStatement> InsertStatement::Copy() const {
  return unique_ptr<InsertStatement>(new InsertStatement(*this));
}

ExpressionListRef *InsertStatement::GetValuesList() const {
  if (select_statement_->node_->type_ != QueryNodeType::SELECT_NODE) {
    return nullptr;
  }
  auto &node = (SelectNode &)*select_statement_->node_;
  if (node.where_clause_ || node.qualify_ || node.having_) {
    return nullptr;
  }
  if (!node.groups_.grouping_sets_.empty()) {
    return nullptr;
  }
  if (node.select_list_.size() != 1 || node.select_list_[0]->type_ != ExpressionType::STAR) {
    return nullptr;
  }
  if (!node.from_table_ || node.from_table_->type_ != TableReferenceType::EXPRESSION_LIST) {
    return nullptr;
  }
  return reinterpret_cast<ExpressionListRef *>(node.from_table_.get());
}

}  // namespace bustub
