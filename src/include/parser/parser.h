//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/parser.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "catalog/column.h"
#include "common/enums/type_id.h"
#include "nodes/parsenodes.hpp"
#include "parser/simplified_token.h"
#include "parser/sql_statement.h"
#include "parser/tokens.h"
#include "pg_definitions.hpp"
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

  //! Returns a list of all keywords in the parser
  static vector<ParserKeyword> KeywordList();

  //! Tokenize a query, returning the raw tokens together with their locations
  static vector<SimplifiedToken> Tokenize(const string &query);

  //! Transforms a Postgres parse tree into a set of SQL Statements
  bool TransformParseTree(bustub_libpgquery::PGList *tree, std::vector<std::unique_ptr<SQLStatement>> &statements);

  string NodetypeToString(bustub_libpgquery::PGNodeTag type);

  Column TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef);

  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatement(bustub_libpgquery::PGNode *stmt);

  //! Transforms a Postgres statement into a single SQL statement
  unique_ptr<SQLStatement> TransformStatementInternal(bustub_libpgquery::PGNode *stmt);

  //! Transform a Postgres TypeName string into a TypeId
  TypeId TransformTypeId(bustub_libpgquery::PGTypeName *type_name);

  std::string PGNodetypeToString(bustub_libpgquery::PGNodeTag type);

  Value TransformConstant(bustub_libpgquery::PGAConst *c);

  void TransformExpressionList(bustub_libpgquery::PGList &list, vector<Value> &result);
};

}  // namespace bustub
