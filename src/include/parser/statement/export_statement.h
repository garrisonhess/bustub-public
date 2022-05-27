//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/statement/export_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/copy_info.h"
#include "parser/parsed_expression.h"
#include "parser/sql_statement.h"

namespace bustub {

class ExportStatement : public SQLStatement {
 public:
  explicit ExportStatement(unique_ptr<CopyInfo> info);

  unique_ptr<CopyInfo> info;

 protected:
  ExportStatement(const ExportStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
