//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/qualified_name.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>
#include <vector>

#include "common/constants.h"
#include "common/exception.h"

// #include "parser/keyword_helper.h"

namespace bustub {
using std::string;
using std::vector;

struct QualifiedName {
  string schema_;
  string name_;

  //! Parse the (optional) schema and a name from a string in the format of e.g. "schema"."table"; if there is no dot
  //! the schema will be set to INVALID_SCHEMA
  static QualifiedName Parse(const string &input) {
    string schema;
    string name;
    uint64_t idx = 0;
    vector<string> entries;
    string entry;
  normal:
    //! quote
    for (; idx < input.size(); idx++) {
      if (input[idx] == '"') {
        idx++;
        goto quoted;
      } else if (input[idx] == '.') {
        goto separator;
      }
      entry += input[idx];
    }
    goto end;
  separator:
    entries.push_back(entry);
    entry = "";
    idx++;
    goto normal;
  quoted:
    //! look for another quote
    for (; idx < input.size(); idx++) {
      if (input[idx] == '"') {
        //! unquote
        idx++;
        goto normal;
      }
      entry += input[idx];
    }
    throw Exception("Unterminated quote in qualified name!");
  end:
    if (entries.empty()) {
      schema = INVALID_SCHEMA;
      name = entry;
    } else if (entries.size() == 1) {
      schema = entries[0];
      name = entry;
    } else {
      throw Exception("Expected schema.entry or entry: too many entries found");
    }
    return QualifiedName{schema, name};
  }
};

struct QualifiedColumnName {
  QualifiedColumnName() = default;
  QualifiedColumnName(string table_p, string column_p) : table_(move(table_p)), column_(move(column_p)) {}

  string schema_;
  string table_;
  string column_;
};

}  // namespace bustub
