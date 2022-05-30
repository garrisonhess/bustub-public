#include "parser/query_node.h"

#include <unordered_map>

#include "common/field_writer.h"
#include "parser/query_node/select_node.h"
#include "parser/transformer.h"

namespace bustub {

string QueryNode::ResultModifiersToString() const {
  string result;
  for (const auto &modifier_idx : modifiers_) {
    auto &modifier = *modifier_idx;
    if (modifier.type_ == ResultModifierType::ORDER_MODIFIER) {
      auto &order_modifier = (OrderModifier &)modifier;
      result += " ORDER BY ";
      for (uint64_t k = 0; k < order_modifier.orders_.size(); k++) {
        if (k > 0) {
          result += ", ";
        }
        result += order_modifier.orders_[k].ToString();
      }
    } else if (modifier.type_ == ResultModifierType::LIMIT_MODIFIER) {
      auto &limit_modifier = (LimitModifier &)modifier;
      if (limit_modifier.limit_) {
        result += " LIMIT " + limit_modifier.limit_->ToString();
      }
      if (limit_modifier.offset_) {
        result += " OFFSET " + limit_modifier.offset_->ToString();
      }
    }
  }
  return result;
}

bool QueryNode::Equals(const QueryNode *other) const {
  if (other == nullptr) {
    return false;
  }
  if (this == other) {
    return true;
  }
  if (other->type_ != this->type_) {
    return false;
  }
  if (modifiers_.size() != other->modifiers_.size()) {
    return false;
  }
  for (uint64_t i = 0; i < modifiers_.size(); i++) {
    if (!modifiers_[i]->Equals(other->modifiers_[i].get())) {
      return false;
    }
  }
  return other->type_ == type_;
}

void QueryNode::CopyProperties(QueryNode &other) const {
  for (auto &modifier : modifiers_) {
    other.modifiers_.push_back(modifier->Copy());
  }
}

void QueryNode::Serialize(Serializer &main_serializer) const {
  FieldWriter writer(main_serializer);
  writer.WriteField<QueryNodeType>(type_);
  writer.WriteSerializableList(modifiers_);
  Serialize(writer);
  writer.Finalize();
}

unique_ptr<QueryNode> QueryNode::Deserialize(Deserializer &main_source) {
  FieldReader reader(main_source);
  auto type = reader.ReadRequired<QueryNodeType>();
  auto modifiers = reader.ReadRequiredSerializableList<ResultModifier>();
  unique_ptr<QueryNode> result;

  switch (type) {
    case QueryNodeType::SELECT_NODE:
      result = SelectNode::Deserialize(reader);
      break;
    default:
      throw Exception("Could not deserialize Query Node: unknown type!");
  }
  result->modifiers_ = std::move(modifiers);
  reader.Finalize();
  return result;
}

}  // namespace bustub
