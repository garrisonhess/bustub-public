#include "main/prepared_statement_data.h"
// #include "execution/physical_operator.h"
#include "common/exception.h"
#include "parser/sql_statement.h"

namespace bustub {

PreparedStatementData::PreparedStatementData(StatementType type) : statement_type_(type) {}

PreparedStatementData::~PreparedStatementData() = default;

}  // namespace bustub
