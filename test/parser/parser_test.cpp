

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
  parser.ParseQuery("SELECT * FROM temp;");

  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
    LOG_INFO("Statement data: n_param: %d", stmt->n_param_);
    LOG_INFO("Statement data: query: %s", stmt->query_.c_str());
    LOG_INFO("Statement data: type: %d", static_cast<int>(stmt->type_));
  }
}

TEST(ParserTest, BadQueryParserTest) {
  Parser parser;
  parser.ParseQuery("SELfasddfasECT fasdfasd* FdfasfasdsdfasdfasdfaROM temp;");
  for (auto &&stmt : parser.statements_) {
    LOG_INFO("Parsed statement: %s", stmt->ToString().c_str());
  }
}

}  // namespace bustub
