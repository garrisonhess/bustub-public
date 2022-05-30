#include "parser/query_node/select_node.h"

#include "common/field_writer.h"
#include "common/logger.h"
#include "parser/keyword_helper.h"
// #include "parser/expression_util.h"

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
  if (where_clause_) {
    result += " WHERE " + where_clause_->ToString();
  }
  if (!groups_.grouping_sets_.empty()) {
    result += " GROUP BY ";
    // if we are dealing with multiple grouping sets, we have to add a few additional brackets
    bool grouping_sets = groups_.grouping_sets_.size() > 1;
    if (grouping_sets) {
      result += "GROUPING SETS (";
    }
    for (uint64_t i = 0; i < groups_.grouping_sets_.size(); i++) {
      auto &grouping_set = groups_.grouping_sets_[i];
      if (i > 0) {
        result += ",";
      }
      if (grouping_set.empty()) {
        result += "()";
        continue;
      }
      if (grouping_sets) {
        result += "(";
      }
      bool first = true;
      for (auto &grp : grouping_set) {
        if (!first) {
          result += ", ";
        }
        result += groups_.group_expressions_[grp]->ToString();
        first = false;
      }
      if (grouping_sets) {
        result += ")";
      }
    }
    if (grouping_sets) {
      result += ")";
    }
  }
  if (having_) {
    result += " HAVING " + having_->ToString();
  }
  if (qualify_) {
    result += " QUALIFY " + qualify_->ToString();
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
  // if (!ExpressionUtil::ListEquals(select_list_, other->select_list_)) {
  // 	return false;
  // }
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
  // WHERE
  if (!BaseExpression::Equals(where_clause_.get(), other->where_clause_.get())) {
    return false;
  }
  // GROUP BY
  // if (!ExpressionUtil::ListEquals(groups_.group_expressions_, other->groups_.group_expressions_)) {
  // 	return false;
  // }
  if (groups_.grouping_sets_ != other->groups_.grouping_sets_) {
    return false;
  }
  // HAVING
  if (!BaseExpression::Equals(having_.get(), other->having_.get())) {
    return false;
  }
  // QUALIFY
  if (!BaseExpression::Equals(qualify_.get(), other->qualify_.get())) {
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
  result->where_clause_ = where_clause_ ? where_clause_->Copy() : nullptr;
  // groups
  for (auto &group : groups_.group_expressions_) {
  	result->groups_.group_expressions_.push_back(group->Copy());
  }
  result->groups_.grouping_sets_ = groups_.grouping_sets_;
  result->having_ = having_ ? having_->Copy() : nullptr;
  result->qualify_ = qualify_ ? qualify_->Copy() : nullptr;
  this->CopyProperties(*result);
  return result;
}

void SelectNode::Serialize(FieldWriter &writer) const {
  writer.WriteSerializableList(select_list_);
  writer.WriteOptional(from_table_);
  writer.WriteOptional(where_clause_);
  writer.WriteSerializableList(groups_.group_expressions_);
  writer.WriteField<uint32_t>(groups_.grouping_sets_.size());
  auto &serializer = writer.GetSerializer();
  for (auto &grouping_set : groups_.grouping_sets_) {
  	serializer.Write<uint64_t>(grouping_set.size());
  	for (auto &idx : grouping_set) {
  		serializer.Write<uint64_t>(idx);
  	}
  }
  writer.WriteOptional(having_);
  writer.WriteOptional(qualify_);
}

unique_ptr<QueryNode> SelectNode::Deserialize(FieldReader &reader) {
  auto result = std::make_unique<SelectNode>();
  result->select_list_ = reader.ReadRequiredSerializableList<ParsedExpression>();
  result->from_table_ = reader.ReadOptional<TableRef>(nullptr);
  result->where_clause_ = reader.ReadOptional<ParsedExpression>(nullptr);
  result->groups_.group_expressions_ = reader.ReadRequiredSerializableList<ParsedExpression>();

  auto grouping_set_count = reader.ReadRequired<uint32_t>();
  auto &source = reader.GetSource();
  for (uint64_t set_idx = 0; set_idx < grouping_set_count; set_idx++) {
  	auto set_entries = source.Read<uint64_t>();
  	GroupingSet grouping_set;
  	for (uint64_t i = 0; i < set_entries; i++) {
  		grouping_set.insert(source.Read<uint64_t>());
  	}
  	result->groups_.grouping_sets_.push_back(grouping_set);
  }
  result->having_ = reader.ReadOptional<ParsedExpression>(nullptr);
  result->qualify_ = reader.ReadOptional<ParsedExpression>(nullptr);
  return result;
}

}  // namespace bustub
