#include "parser/base_expression.h"

#include "common/printer.h"
#include <string>


namespace bustub {

void BaseExpression::Print() const {
	Printer::Print(ToString());
}

std::string BaseExpression::GetName() const {
	return !alias_.empty() ? alias_ : ToString();
}

bool BaseExpression::Equals(const BaseExpression *other) const {
	if (other == nullptr) {
		return false;
	}
	if (this->expression_class_ != other->expression_class_ || this->type_ != other->type_) {
		return false;
	}
	return true;
}

void BaseExpression::Verify() const {
}

} // namespace bustub
