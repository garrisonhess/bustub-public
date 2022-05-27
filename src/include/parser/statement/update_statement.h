//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/update_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"
#include "parser/tableref.h"

namespace bustub {

class UpdateStatement : public SQLStatement {
 public:
  UpdateStatement();

  unique_ptr<ParsedExpression> condition;
  unique_ptr<TableRef> table;
  unique_ptr<TableRef> from_table;
  vector<string> columns;
  vector<unique_ptr<ParsedExpression>> expressions;
  //! keep track of optional returningList if statement contains a RETURNING keyword
  vector<unique_ptr<ParsedExpression>> returning_list;

 protected:
  UpdateStatement(const UpdateStatement &other);

 public:
  string ToString() const override;
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
