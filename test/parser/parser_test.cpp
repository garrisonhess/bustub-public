

//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// parser_test.cpp
//
// Identification: test/parser/parser_test.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "parser/parser.h"

#include "gtest/gtest.h"
#include "test_util.h"  // NOLINT

namespace bustub {

TEST(ParserTest, SimpleParserTest) {
  Parser parser;
  parser.ParseQuery("SELECT * FROM test_table;");

  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
    LOG_INFO("Statement data: query: %s", stmt->query_.c_str());
    LOG_INFO("Statement data: type: %d", static_cast<int>(stmt->type_));
  }
}

TEST(ParserTest, CreateParserTest) {
  Parser parser;
  parser.ParseQuery("CREATE TABLE test_table (column1 int, column2 decimal, column3 varchar);");

  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
    LOG_INFO("Statement data: query: %s", stmt->query_.c_str());
    LOG_INFO("Statement data: type: %d", static_cast<int>(stmt->type_));
  }
}

TEST(ParserTest, InsertParserTest) {
  Parser parser;
  parser.ParseQuery("INSERT INTO test_table (column1, column2, column3) VALUES (1, 1.0, '1'));");

  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
    LOG_INFO("Statement data: query: %s", stmt->query_.c_str());
    LOG_INFO("Statement data: type: %d", static_cast<int>(stmt->type_));
  }
}

TEST(ParserTest, BadQueryParserTest) {
  Parser parser;
  parser.ParseQuery("SELECT * F_ROM test_table;");
  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
  }
}

}  // namespace bustub
