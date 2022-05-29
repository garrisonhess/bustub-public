//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/statement/create_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/create_info.h"
#include "parser/sql_statement.h"

namespace bustub {

class CreateStatement : public SQLStatement {
 public:
  CreateStatement();

  unique_ptr<CreateInfo> info_;

 protected:
  CreateStatement(const CreateStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
