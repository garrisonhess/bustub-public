//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/statement/explain_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

enum class ExplainType : uint8_t { EXPLAIN_STANDARD, EXPLAIN_ANALYZE };

class ExplainStatement : public SQLStatement {
 public:
  explicit ExplainStatement(unique_ptr<SQLStatement> stmt, ExplainType explain_type = ExplainType::EXPLAIN_STANDARD);

  unique_ptr<SQLStatement> stmt;
  ExplainType explain_type;

 protected:
  ExplainStatement(const ExplainStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
