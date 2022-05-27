#include "parser/statement/insert_statement.h"
#include "common/constants.h"
#include "parser/query_node/select_node.h"
#include "parser/tableref/expressionlistref.h"

namespace bustub {

InsertStatement::InsertStatement() : SQLStatement(StatementType::INSERT_STATEMENT), schema(DEFAULT_SCHEMA) {
}

InsertStatement::InsertStatement(const InsertStatement &other)
    : SQLStatement(other),
      select_statement(unique_ptr<SelectStatement>(static_cast<SelectStatement*>((other.select_statement->Copy().release())))),
      columns(other.columns), table(other.table), schema(other.schema) {
}

string InsertStatement::ToString() const {
	string result;
	// result = "INSERT INTO ";
	// if (!schema.empty()) {
	// 	result += KeywordHelper::WriteOptionallyQuoted(schema) + ".";
	// }
	// result += KeywordHelper::WriteOptionallyQuoted(table);
	// if (!columns.empty()) {
	// 	result += " (";
	// 	for (uint64_t i = 0; i < columns.size(); i++) {
	// 		if (i > 0) {
	// 			result += ", ";
	// 		}
	// 		result += KeywordHelper::WriteOptionallyQuoted(columns[i]);
	// 	}
	// 	result += " )";
	// }
	// result += " ";
	// auto values_list = GetValuesList();
	// if (values_list != nullptr) {
	// 	values_list->alias_ = string();
	// 	result += values_list->ToString();
	// } else {
	// 	result += select_statement->ToString();
	// }
	// if (!returning_list.empty()) {
	// 	result += " RETURNING ";
	// 	for (uint64_t i = 0; i < returning_list.size(); i++) {
	// 		if (i > 0) {
	// 			result += ", ";
	// 		}
	// 		result += returning_list[i]->ToString();
	// 	}
	// }
	return result;
}

unique_ptr<SQLStatement> InsertStatement::Copy() const {
	return unique_ptr<InsertStatement>(new InsertStatement(*this));
}

ExpressionListRef *InsertStatement::GetValuesList() const {
	if (select_statement->node->type_ != QueryNodeType::SELECT_NODE) {
		return nullptr;
	}
	auto &node = (SelectNode &)*select_statement->node;
	if (node.where_clause_ || node.qualify_ || node.having_) {
		return nullptr;
	}
	// if (!node.cte_map.empty()) {
	// 	return nullptr;
	// }
	if (!node.groups_.grouping_sets_.empty()) {
		return nullptr;
	}
	// if (node.aggregate_handling != AggregateHandling::STANDARD_HANDLING) {
	// 	return nullptr;
	// }
	if (node.select_list_.size() != 1 || node.select_list_[0]->type_ != ExpressionType::STAR) {
		return nullptr;
	}
	if (!node.from_table_ || node.from_table_->type_ != TableReferenceType::EXPRESSION_LIST) {
		return nullptr;
	}
	return (ExpressionListRef *)node.from_table_.get();
}

} // namespace bustub
