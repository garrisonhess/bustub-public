#include "parser/statement/select_statement.h"

#include "common/serializer.h"

namespace bustub {

SelectStatement::SelectStatement(const SelectStatement &other) : SQLStatement(other), node_(other.node_->Copy()) {}

unique_ptr<SQLStatement> SelectStatement::Copy() const {
  return unique_ptr<SelectStatement>(new SelectStatement(*this));
}

void SelectStatement::Serialize(Serializer &serializer) const { node_->Serialize(serializer); }

unique_ptr<SelectStatement> SelectStatement::Deserialize(Deserializer &source) {
  auto result = std::make_unique<SelectStatement>();
  result->node_ = QueryNode::Deserialize(source);
  return result;
}

bool SelectStatement::Equals(const SQLStatement *other_p) const {
  if (type_ != other_p->type_) {
    return false;
  }
  auto other = (SelectStatement *)other_p;
  return node_->Equals(other->node_.get());
}

string SelectStatement::ToString() const { return node_->ToString(); }

}  // namespace bustub
