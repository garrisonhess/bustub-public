#include "parser/parser.h"

#include <iostream>

#include "common/logger.h"
#include "parser/transformer.h"

namespace bustub {
using bustub_libpgquery::PGKeywordCategory;
using bustub_libpgquery::PGSimplifiedTokenType;
using std::move;

void Parser::ParseQuery(const std::string &query) {
  Transformer transformer(nullptr);
  {
    PostgresParser parser;
    parser.Parse(query);
    if (!parser.success) {
      LOG_INFO("Query failed to parse!");
      throw Exception("Query failed to parse!");
      return;
    }

    if (parser.parse_tree == nullptr) {
      LOG_INFO("parser received empty statement");
      return;
    }

    // if it succeeded, we transform the Postgres parse tree into a list of
    // SQLStatements
    transformer.TransformParseTree(parser.parse_tree, statements_);
  }

  // if (!statements_.empty()) {
  //   auto &last_statement = statements_.back();
  //   last_statement->stmt_length_ = query.size() - last_statement->stmt_location_;
  //   for (auto &statement : statements_) {
  //     statement->query_ = query;
  //     LOG_INFO("parsed query: %s", statement->query_.c_str());
  //     if (statement->type_ == StatementType::CREATE_STATEMENT) {
  //       auto &create = (CreateStatement &)*statement;
  //       create.sql_ = query.substr(statement->stmt_location_, statement->stmt_length_);
  //     }
  //   }
  // }
}

}  // namespace bustub
