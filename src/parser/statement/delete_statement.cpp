#include "parser/statement/delete_statement.h"

namespace bustub {

DeleteStatement::DeleteStatement() : SQLStatement(StatementType::DELETE_STATEMENT) {}

DeleteStatement::DeleteStatement(const DeleteStatement &other) : SQLStatement(other), table_(other.table_->Copy()) {
  if (other.condition_) {
    condition_ = other.condition_->Copy();
  }
  for (const auto &using_clause : other.using_clauses_) {
    using_clauses_.push_back(using_clause->Copy());
  }
}

string DeleteStatement::ToString() const {
  string result;
  result += "DELETE FROM ";
  result += table_->ToString();
  if (!using_clauses_.empty()) {
    result += " USING ";
    for (uint64_t i = 0; i < using_clauses_.size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += using_clauses_[i]->ToString();
    }
  }
  if (condition_) {
    result += " WHERE " + condition_->ToString();
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

unique_ptr<SQLStatement> DeleteStatement::Copy() const {
  return unique_ptr<DeleteStatement>(new DeleteStatement(*this));
}

}  // namespace bustub
