//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/parser.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

#include "parser/parsed_expression.h"
#include "parser/simplified_token.h"
#include "parser/sql_statement.h"
#include "postgres_parser.hpp"

#include "parser/column_definition.h"
#include "parser/query_node.h"

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
  //   Parser(ParserOptions options = ParserOptions());
  Parser() = default;

  //! The parsed SQL statements from an invocation to ParseQuery.
  vector<unique_ptr<SQLStatement>> statements_;

  //! Attempts to parse a query into a series of SQL statements. Returns
  //! whether or not the parsing was successful. If the parsing was
  //! successful, the parsed statements will be stored in the statements
  //! variable.
  void ParseQuery(const string &query);

  //! Tokenize a query, returning the raw tokens together with their locations
  static vector<SimplifiedToken> Tokenize(const string &query);

  //! Returns true if the given text matches a keyword of the parser
  static bool IsKeyword(const string &text);

  //! Returns a list of all keywords in the parser
  static vector<ParserKeyword> KeywordList();

  //! Parses a list of expressions (i.e. the list found in a SELECT clause)
  static vector<unique_ptr<ParsedExpression>> ParseExpressionList(const string &select_list);

  //! Parses a list as found in an ORDER BY expression (i.e. including optional ASCENDING/DESCENDING modifiers)
  static vector<OrderByNode> ParseOrderList(const string &select_list);

  //! Parses an update list (i.e. the list found in the SET clause of an UPDATE statement)
  static void ParseUpdateList(const string &update_list, vector<string> &update_columns,
                              vector<unique_ptr<ParsedExpression>> &expressions);

  //! Parses a VALUES list (i.e. the list of expressions after a VALUES clause)
  static vector<vector<unique_ptr<ParsedExpression>>> ParseValuesList(const string &value_list);

  //! Parses a column list (i.e. as found in a CREATE TABLE statement)
  static vector<ColumnDefinition> ParseColumnList(const string &column_list);
};

}  // namespace bustub
