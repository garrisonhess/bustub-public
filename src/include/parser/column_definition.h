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
using std::unique_ptr;

//! A column of a table.
class ColumnDefinition {
 public:
  ColumnDefinition(string name, Type type);
  ColumnDefinition(string name, Type type, unique_ptr<ParsedExpression> default_value);

  //! The name of the entry
  string name_;
  //! The index of the column in the table
  int64_t oid_;
  //! The type of the column
  Type type_;
  //! The default value of the column (if any)
  unique_ptr<ParsedExpression> default_value_;

 public:
  ColumnDefinition Copy() const;

  void Serialize(Serializer &serializer) const;
  static ColumnDefinition Deserialize(Deserializer &source);
};

}  // namespace bustub
