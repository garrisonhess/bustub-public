#include "parser/parser.h"

#include <iostream>

#include "common/logger.h"
#include "parser/query_node/select_node.h"
#include "parser/statement/create_statement.h"
#include "parser/statement/select_statement.h"
#include "parser/statement/update_statement.h"
#include "parser/tableref/expressionlistref.h"
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

  if (!statements_.empty()) {
    auto &last_statement = statements_.back();
    last_statement->stmt_length_ = query.size() - last_statement->stmt_location_;
    for (auto &statement : statements_) {
      statement->query_ = query;
      LOG_INFO("parsed query: %s", statement->query_.c_str());
      if (statement->type_ == StatementType::CREATE_STATEMENT) {
        auto &create = (CreateStatement &)*statement;
        create.sql_ = query.substr(statement->stmt_location_, statement->stmt_length_);
      }
    }
  }
}

std::vector<SimplifiedToken> Parser::Tokenize(const std::string &query) {
  auto pg_tokens = PostgresParser::Tokenize(query);
  std::vector<SimplifiedToken> result;
  result.reserve(pg_tokens.size());
  for (auto &pg_token : pg_tokens) {
    SimplifiedToken token;
    switch (pg_token.type) {
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_IDENTIFIER:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_IDENTIFIER;
        break;
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_NUMERIC_CONSTANT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_NUMERIC_CONSTANT;
        break;
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_STRING_CONSTANT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_STRING_CONSTANT;
        break;
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_OPERATOR:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_OPERATOR;
        break;
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_KEYWORD:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_KEYWORD;
        break;
      // comments are not supported by our tokenizer right now
      case PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_COMMENT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_COMMENT;
        break;
      default:
        throw Exception("Unrecognized token category");
    }
    token.start_ = pg_token.start;
    result.push_back(token);
  }
  return result;
}

bool Parser::IsKeyword(const std::string &text) { return PostgresParser::IsKeyword(text); }

std::vector<ParserKeyword> Parser::KeywordList() {
  auto keywords = PostgresParser::KeywordList();
  std::vector<ParserKeyword> result;
  for (auto &kw : keywords) {
    ParserKeyword res;
    res.name_ = kw.text;
    switch (kw.category) {
      case PGKeywordCategory::PG_KEYWORD_RESERVED:
        res.category_ = KeywordCategory::KEYWORD_RESERVED;
        break;
      case PGKeywordCategory::PG_KEYWORD_UNRESERVED:
        res.category_ = KeywordCategory::KEYWORD_UNRESERVED;
        break;
      case PGKeywordCategory::PG_KEYWORD_TYPE_FUNC:
        res.category_ = KeywordCategory::KEYWORD_TYPE_FUNC;
        break;
      case PGKeywordCategory::PG_KEYWORD_COL_NAME:
        res.category_ = KeywordCategory::KEYWORD_COL_NAME;
        break;
      default:
        throw Exception("Unrecognized keyword category");
    }
    result.push_back(res);
  }
  return result;
}

std::vector<std::unique_ptr<ParsedExpression>> Parser::ParseExpressionList(const std::string &select_list) {
  // construct a mock query prefixed with SELECT
  std::string mock_query = "SELECT " + select_list;
  // parse the query
  Parser parser;
  parser.ParseQuery(mock_query);
  // check the statements
  if (parser.statements_.size() != 1 || parser.statements_[0]->type_ != StatementType::SELECT_STATEMENT) {
    throw Exception("Expected a single SELECT statement");
  }
  auto &select = (SelectStatement &)*parser.statements_[0];
  if (select.node_->type_ != QueryNodeType::SELECT_NODE) {
    throw Exception("Expected a single SELECT node");
  }
  auto &select_node = (SelectNode &)*select.node_;
  return move(select_node.select_list_);
}

std::vector<OrderByNode> Parser::ParseOrderList(const std::string &select_list) {
  // construct a mock query
  std::string mock_query = "SELECT * FROM tbl ORDER BY " + select_list;
  // parse the query
  Parser parser;
  parser.ParseQuery(mock_query);
  // check the statements
  if (parser.statements_.size() != 1 || parser.statements_[0]->type_ != StatementType::SELECT_STATEMENT) {
    throw Exception("Expected a single SELECT statement");
  }
  auto &select = (SelectStatement &)*parser.statements_[0];
  if (select.node_->type_ != QueryNodeType::SELECT_NODE) {
    throw Exception("Expected a single SELECT node");
  }
  auto &select_node = (SelectNode &)*select.node_;
  if (select_node.modifiers_.empty() || select_node.modifiers_[0]->type_ != ResultModifierType::ORDER_MODIFIER ||
      select_node.modifiers_.size() != 1) {
    throw Exception("Expected a single ORDER clause");
  }
  auto &order = (OrderModifier &)*select_node.modifiers_[0];
  return move(order.orders_);
}

void Parser::ParseUpdateList(const std::string &update_list, std::vector<std::string> &update_columns,
                             std::vector<std::unique_ptr<ParsedExpression>> &expressions) {
  // construct a mock query
  std::string mock_query = "UPDATE tbl SET " + update_list;
  // parse the query
  Parser parser;
  parser.ParseQuery(mock_query);
  // check the statements
  if (parser.statements_.size() != 1 || parser.statements_[0]->type_ != StatementType::UPDATE_STATEMENT) {
    throw Exception("Expected a single UPDATE statement");
  }
  auto &update = (UpdateStatement &)*parser.statements_[0];
  update_columns = move(update.columns_);
  expressions = move(update.expressions_);
}

std::vector<std::vector<std::unique_ptr<ParsedExpression>>> Parser::ParseValuesList(const std::string &value_list) {
  // construct a mock query
  std::string mock_query = "VALUES " + value_list;
  // parse the query
  Parser parser;
  parser.ParseQuery(mock_query);
  // check the statements
  if (parser.statements_.size() != 1 || parser.statements_[0]->type_ != StatementType::SELECT_STATEMENT) {
    throw Exception("Expected a single SELECT statement");
  }
  auto &select = (SelectStatement &)*parser.statements_[0];
  if (select.node_->type_ != QueryNodeType::SELECT_NODE) {
    throw Exception("Expected a single SELECT node");
  }
  auto &select_node = (SelectNode &)*select.node_;
  if (!select_node.from_table_ || select_node.from_table_->type_ != TableReferenceType::EXPRESSION_LIST) {
    throw Exception("Expected a single VALUES statement");
  }
  auto &values_list = (ExpressionListRef &)*select_node.from_table_;
  return move(values_list.values_);
}

vector<ColumnDefinition> Parser::ParseColumnList(const string &column_list) {
  string mock_query = "CREATE TABLE blabla (" + column_list + ")";
  Parser parser;
  parser.ParseQuery(mock_query);
  if (parser.statements_.size() != 1 || parser.statements_[0]->type_ != StatementType::CREATE_STATEMENT) {
    throw Exception("Expected a single CREATE statement");
  }

  auto &create = dynamic_cast<CreateStatement &>(*parser.statements_[0]);
  if (create.type_ != CatalogType::TABLE_ENTRY) {
    throw Exception("Expected a single CREATE TABLE statement");
  }

  return create.columns_;
}

}  // namespace bustub
