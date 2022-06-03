//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/parser.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

#include "parser/sql_statement.h"
#include "postgres_parser.hpp"

namespace bustub_libpgquery {
struct PGNode;
struct PGList;
}  // namespace bustub_libpgquery

#include <string>

namespace bustub {
using std::vector;

//! The parser is responsible for parsing the query and converting it into a set
//! of parsed statements. The parsed statements can then be converted into a
//! plan and executed.
class Parser {
 public:
  Parser() = default;

  //! The parsed SQL statements from an invocation to ParseQuery.
  vector<unique_ptr<SQLStatement>> statements_;

  //! Attempts to parse a query into a series of SQL statements. Returns
  //! whether or not the parsing was successful. If the parsing was
  //! successful, the parsed statements will be stored in the statements
  //! variable.
  void ParseQuery(const string &query);

  //! Returns true if the given text matches a keyword of the parser
  static bool IsKeyword(const string &text);
};

}  // namespace bustub
