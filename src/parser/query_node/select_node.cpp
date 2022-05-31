#include "parser/query_node/select_node.h"

#include "common/field_writer.h"
#include "common/logger.h"
#include "parser/expression_util.h"
#include "parser/keyword_helper.h"

namespace bustub {

SelectNode::SelectNode() : QueryNode(QueryNodeType::SELECT_NODE) {}

string SelectNode::ToString() const {
  string result;
  result += "SELECT ";

  // search for a distinct modifier
  for (const auto &modifier : modifiers_) {
    if (modifier->type_ == ResultModifierType::DISTINCT_MODIFIER) {
      auto &distinct_modifier = static_cast<DistinctModifier &>(*modifier);
      result += "DISTINCT ";
      if (!distinct_modifier.distinct_on_targets_.empty()) {
        result += "ON (";
        for (uint64_t k = 0; k < distinct_modifier.distinct_on_targets_.size(); k++) {
          if (k > 0) {
            result += ", ";
          }
          result += distinct_modifier.distinct_on_targets_[k]->ToString();
        }
        result += ") ";
      }
    }
  }
  for (uint64_t i = 0; i < select_list_.size(); i++) {
    if (i > 0) {
      result += ", ";
    }
    result += select_list_[i]->ToString();
    if (!select_list_[i]->alias_.empty()) {
      result += " AS " + KeywordHelper::WriteOptionallyQuoted(select_list_[i]->alias_);
    }
  }
  if (from_table_ && from_table_->type_ != TableReferenceType::EMPTY) {
    result += " FROM " + from_table_->ToString();
  }
  return result + ResultModifiersToString();
}

bool SelectNode::Equals(const QueryNode *other_p) const {
  if (!QueryNode::Equals(other_p)) {
    return false;
  }
  if (this == other_p) {
    return true;
  }
  auto other = (SelectNode *)other_p;

  // SELECT
  if (!ExpressionUtil::ListEquals(select_list_, other->select_list_)) {
    return false;
  }
  // FROM
  if (from_table_) {
    // we have a FROM clause, compare to the other one
    if (!from_table_->Equals(other->from_table_.get())) {
      return false;
    }
  } else if (other->from_table_) {
    // we don't have a FROM clause, if the other statement has one they are
    // not equal
    return false;
  }
  return true;
}

unique_ptr<QueryNode> SelectNode::Copy() const {
  auto result = std::make_unique<SelectNode>();
  for (auto &child : select_list_) {
    result->select_list_.push_back(child->Copy());
  }
  result->from_table_ = from_table_ ? from_table_->Copy() : nullptr;
  this->CopyProperties(*result);
  return result;
}

void SelectNode::Serialize(FieldWriter &writer) const {
  writer.WriteSerializableList(select_list_);
  writer.WriteOptional(from_table_);
}

unique_ptr<QueryNode> SelectNode::Deserialize(FieldReader &reader) {
  auto result = std::make_unique<SelectNode>();
  result->select_list_ = reader.ReadRequiredSerializableList<ParsedExpression>();
  result->from_table_ = reader.ReadOptional<TableRef>(nullptr);
  return result;
}

}  // namespace bustub
