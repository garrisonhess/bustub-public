//===----------------------------------------------------------------------===//
//                         BusTub
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

  unique_ptr<ParsedExpression> condition_;
  unique_ptr<TableRef> table_;
  unique_ptr<TableRef> from_table_;
  vector<string> columns_;
  vector<unique_ptr<ParsedExpression>> expressions_;
  //! keep track of optional returningList if statement contains a RETURNING keyword
  vector<unique_ptr<ParsedExpression>> returning_list_;

 protected:
  UpdateStatement(const UpdateStatement &other);

 public:
  string ToString() const override;
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
