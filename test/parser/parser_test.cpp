

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
  // Construct query plan
  LOG_INFO("Hi from seqscan test");
  Parser parser;
  parser.ParseQuery("SELECT * FROM temp;");
  parser.ParseQuery("SELfasddfasECT fasdfasd* FdfasfasdsdfasdfasdfaROM temp;");
  // LOG_INFO("parser success: %d", parser.success);
}

}  // namespace bustub
