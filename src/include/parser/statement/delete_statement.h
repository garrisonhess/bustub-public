//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/statement/delete_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"
#include "parser/tableref.h"

namespace bustub {

class DeleteStatement : public SQLStatement {
 public:
  DeleteStatement();

  unique_ptr<ParsedExpression> condition_;
  unique_ptr<TableRef> table_;
  vector<unique_ptr<TableRef>> using_clauses_;
  vector<unique_ptr<ParsedExpression>> returning_list_;

 protected:
  DeleteStatement(const DeleteStatement &other);

 public:
  string ToString() const override;
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
