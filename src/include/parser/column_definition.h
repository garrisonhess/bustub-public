//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/column_definition.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <string>
#include "common/enums/type_id.h"
#include "parser/parsed_expression.h"
#include "type/value.h"

namespace bustub {
using std::string;

//! A column of a table.
class ColumnDefinition {
 public:
  explicit ColumnDefinition(string name, const Type &type);

  //! The name of the entry
  string name_;
  //! The index of the column in the table
  int64_t oid_;
  //! The type of the column
  Type type_;

 public:
  ColumnDefinition Copy() const;

  void Serialize(Serializer &serializer) const;
  static ColumnDefinition Deserialize(Deserializer &source);
};

}  // namespace bustub
