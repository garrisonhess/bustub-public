//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/statement/create_statement.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "common/enums/catalog_type.h"
#include "parser/column_definition.h"
#include "parser/sql_statement.h"
#include "parser/statement/select_statement.h"

namespace bustub {

class CreateStatement : public SQLStatement {
 public:
  CreateStatement();

  //! The to-be-created catalog type
  CatalogType type_;

  //! Table name to insert to
  string table_;

  //! The schema name of the entry
  string schema_;

  //! The SQL string of the CREATE statement
  string sql_;

  //! List of columns of the table
  vector<ColumnDefinition> columns_;

 protected:
  CreateStatement(const CreateStatement &other);

 public:
  unique_ptr<SQLStatement> Copy() const override;
};

}  // namespace bustub
