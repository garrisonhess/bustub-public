#include "parser/result_modifier.h"
#include "common/field_writer.h"
// #include "parser/expression_util.h"

namespace bustub {

bool ResultModifier::Equals(const ResultModifier *other) const {
	if (!other) {
		return false;
	}
	return type_ == other->type_;
}

void ResultModifier::Serialize(Serializer &serializer) const {
	FieldWriter writer(serializer);
	writer.WriteField<ResultModifierType>(type_);
	Serialize(writer);
	writer.Finalize();
}

unique_ptr<ResultModifier> ResultModifier::Deserialize(Deserializer &source) {
	FieldReader reader(source);
	auto type = reader.ReadRequired<ResultModifierType>();

	unique_ptr<ResultModifier> result;
	switch (type) {
	case ResultModifierType::LIMIT_MODIFIER:
		result = LimitModifier::Deserialize(reader);
		break;
	case ResultModifierType::ORDER_MODIFIER:
		result = OrderModifier::Deserialize(reader);
		break;
	case ResultModifierType::DISTINCT_MODIFIER:
		result = DistinctModifier::Deserialize(reader);
		break;
	case ResultModifierType::LIMIT_PERCENT_MODIFIER:
		result = LimitPercentModifier::Deserialize(reader);
		break;
	default:
		throw Exception("Unrecognized ResultModifierType for Deserialization");
	}
	reader.Finalize();
	return result;
}

bool LimitModifier::Equals(const ResultModifier *other_p) const {
	if (!ResultModifier::Equals(other_p)) {
		return false;
	}
	auto &other = (LimitModifier &)*other_p;
	if (!BaseExpression::Equals(limit_.get(), other.limit_.get())) {
		return false;
	}
	if (!BaseExpression::Equals(offset_.get(), other.offset_.get())) {
		return false;
	}
	return true;
}

unique_ptr<ResultModifier> LimitModifier::Copy() const {
	auto copy = std::make_unique<LimitModifier>();
	// if (limit) {
	// 	copy->limit = limit->Copy();
	// }
	// if (offset) {
	// 	copy->offset = offset->Copy();
	// }
	return copy;
}

void LimitModifier::Serialize(FieldWriter &writer) const {
	// writer.WriteOptional(limit);
	// writer.WriteOptional(offset);
}

unique_ptr<ResultModifier> LimitModifier::Deserialize(FieldReader &reader) {
	auto mod = std::make_unique<LimitModifier>();
	// mod->limit_ = reader.ReadOptional<ParsedExpression>(nullptr);
	// mod->offset = reader.ReadOptional<ParsedExpression>(nullptr);
	return mod;
}

bool DistinctModifier::Equals(const ResultModifier *other_p) const {
	// if (!ResultModifier::Equals(other_p)) {
	// 	return false;
	// }
	// auto &other = (DistinctModifier &)*other_p;
	// if (!ExpressionUtil::ListEquals(distinct_on_targets, other.distinct_on_targets)) {
	// 	return false;
	// }
	return true;
}

unique_ptr<ResultModifier> DistinctModifier::Copy() const {
	auto copy = std::make_unique<DistinctModifier>();
	for (auto &expr : distinct_on_targets_) {
		copy->distinct_on_targets_.push_back(expr->Copy());
	}
	return copy;
}

void DistinctModifier::Serialize(FieldWriter &writer) const {
	writer.WriteSerializableList(distinct_on_targets_);
}

unique_ptr<ResultModifier> DistinctModifier::Deserialize(FieldReader &reader) {
	auto mod = std::make_unique<DistinctModifier>();
	// mod->distinct_on_targets_ = reader.ReadRequiredSerializableList<ParsedExpression>();
	return mod;
}

bool OrderModifier::Equals(const ResultModifier *other_p) const {
	if (!ResultModifier::Equals(other_p)) {
		return false;
	}
	auto &other = (OrderModifier &)*other_p;
	if (orders_.size() != other.orders_.size()) {
		return false;
	}
	for (uint64_t i = 0; i < orders_.size(); i++) {
		if (orders_[i].type_ != other.orders_[i].type_) {
			return false;
		}
		if (!BaseExpression::Equals(orders_[i].expression_.get(), other.orders_[i].expression_.get())) {
			return false;
		}
	}
	return true;
}

unique_ptr<ResultModifier> OrderModifier::Copy() const {
	auto copy = std::make_unique<OrderModifier>();
	for (auto &order : orders_) {
		copy->orders_.emplace_back(order.type_, order.null_order_, order.expression_->Copy());
	}
	return copy;
}

string OrderByNode::ToString() const {
	auto str = expression_->ToString();
	switch (type_) {
	case OrderType::ASCENDING:
		str += " ASC";
		break;
	case OrderType::DESCENDING:
		str += " DESC";
		break;
	default:
		break;
	}

	switch (null_order_) {
	case OrderByNullType::NULLS_FIRST:
		str += " NULLS FIRST";
		break;
	case OrderByNullType::NULLS_LAST:
		str += " NULLS LAST";
		break;
	default:
		break;
	}
	return str;
}

void OrderByNode::Serialize(Serializer &serializer) const {
	FieldWriter writer(serializer);
	writer.WriteField<OrderType>(type_);
	writer.WriteField<OrderByNullType>(null_order_);
	writer.WriteSerializable(*expression_);
	writer.Finalize();
}

OrderByNode OrderByNode::Deserialize(Deserializer &source) {
	FieldReader reader(source);
	auto type = reader.ReadRequired<OrderType>();
	auto null_order = reader.ReadRequired<OrderByNullType>();
	auto expression = reader.ReadRequiredSerializable<ParsedExpression>();
	reader.Finalize();
	return OrderByNode(type, null_order, move(expression));
}

void OrderModifier::Serialize(FieldWriter &writer) const {
	writer.WriteRegularSerializableList(orders_);
}

unique_ptr<ResultModifier> OrderModifier::Deserialize(FieldReader &reader) {
	auto mod = std::make_unique<OrderModifier>();
	// mod->orders = reader.ReadRequiredSerializableList<OrderByNode, OrderByNode>();
	return mod;
}

bool LimitPercentModifier::Equals(const ResultModifier *other_p) const {
	if (!ResultModifier::Equals(other_p)) {
		return false;
	}
	auto &other = (LimitPercentModifier &)*other_p;
	if (!BaseExpression::Equals(limit_.get(), other.limit_.get())) {
		return false;
	}
	if (!BaseExpression::Equals(offset_.get(), other.offset_.get())) {
		return false;
	}
	return true;
}

unique_ptr<ResultModifier> LimitPercentModifier::Copy() const {
	auto copy = std::make_unique<LimitPercentModifier>();
	if (limit_) {
		copy->limit_ = limit_->Copy();
	}
	if (offset_) {
		copy->offset_ = offset_->Copy();
	}
	return copy;
}

void LimitPercentModifier::Serialize(FieldWriter &writer) const {
	writer.WriteOptional(limit_);
	writer.WriteOptional(offset_);
}

unique_ptr<ResultModifier> LimitPercentModifier::Deserialize(FieldReader &reader) {
	auto mod = std::make_unique<LimitPercentModifier>();
	// mod->limit_ = reader.ReadOptional<ParsedExpression>(nullptr);
	// mod->offset_ = reader.ReadOptional<ParsedExpression>(nullptr);
	return mod;
}

} // namespace bustub
