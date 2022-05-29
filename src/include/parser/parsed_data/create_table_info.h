//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/parsed_data/create_table_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <utility>

#include "common/constants.h"
#include "parser/column_definition.h"
#include "parser/parsed_data/create_info.h"
#include "parser/statement/select_statement.h"

namespace bustub {
using std::make_unique;
using std::string;

struct CreateTableInfo : public CreateInfo {
  CreateTableInfo() : CreateInfo(CatalogType::TABLE_ENTRY, INVALID_SCHEMA) {}
  CreateTableInfo(string schema, string name)
      : CreateInfo(CatalogType::TABLE_ENTRY, std::move(schema)), table_(std::move(name)) {}

  //! Table name to insert to
  string table_;
  //! List of columns of the table
  vector<ColumnDefinition> columns_;
  //! CREATE TABLE from QUERY
  unique_ptr<SelectStatement> query_;

 public:
  unique_ptr<CreateInfo> Copy() const override {
    auto result = make_unique<CreateTableInfo>(schema_, table_);
    CopyProperties(*result);
    for (auto &column : columns_) {
      result->columns_.push_back(column.Copy());
    }
    if (query_) {
      result->query_ = unique_ptr<SelectStatement>(static_cast<SelectStatement *>(query_->Copy().release()));
    }
    return result;
  }
};

}  // namespace bustub
