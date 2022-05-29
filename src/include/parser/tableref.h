//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/tableref.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include <vector>

#include <memory>
#include "common/enums/tableref_type.h"
#include "common/field_writer.h"

// #include "parser/parsed_data/sample_options.h"

namespace bustub {
using std::string;
using std::unique_ptr;
using std::vector;
class Deserializer;
class Serializer;

//! Represents a generic expression that returns a table.
class TableRef {
 public:
  explicit TableRef(TableReferenceType type) : type_(type) {}
  virtual ~TableRef() = default;

  TableReferenceType type_;
  string alias_;
  //! The location in the query (if any)
  int64_t query_location_ = -1;

 public:
  //! Convert the object to a string
  virtual string ToString() const = 0;
  string BaseToString(string result) const;
  string BaseToString(string result, const vector<string> &column_name_alias) const;
  void Print();

  virtual bool Equals(const TableRef *other) const;

  virtual unique_ptr<TableRef> Copy() = 0;

  //! Serializes a TableRef to a stand-alone binary blob
  void Serialize(Serializer &serializer) const;

  //! Serializes a TableRef to a stand-alone binary blob
  virtual void Serialize(FieldWriter &writer) const = 0;

  //! Deserializes a blob back into a TableRef
  static unique_ptr<TableRef> Deserialize(Deserializer &source);

  //! Copy the properties of this table ref to the target
  void CopyProperties(TableRef &target) const;
};
}  // namespace bustub
