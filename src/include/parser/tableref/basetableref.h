//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/tableref/basetableref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include <vector>
#include "common/constants.h"
#include "parser/tableref.h"

namespace bustub {
using std::unique_ptr;

//! Represents a TableReference to a base table in the schema
class BaseTableRef : public TableRef {
 public:
  BaseTableRef() : TableRef(TableReferenceType::BASE_TABLE) {}

  //! Schema name
  string schema_name_{};
  //! Table name
  string table_name_;
  //! Aliases for the column names
  vector<string> column_name_alias_;

 public:
  string ToString() const override;
  bool Equals(const TableRef *other_p) const override;

  unique_ptr<TableRef> Copy() override;

  //! Serializes a blob into a BaseTableRef
  void Serialize(FieldWriter &serializer) const override;
  //! Deserializes a blob back into a BaseTableRef
  static unique_ptr<TableRef> Deserialize(FieldReader &source);
};
}  // namespace bustub
