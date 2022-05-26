

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

#include <memory>
#include <numeric>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "parser/parser.h"

#include "gtest/gtest.h"
#include "storage/table/tuple.h"
#include "test_util.h"  // NOLINT
#include "type/value_factory.h"

namespace bustub {

TEST(ParserTest, SimpleParserTest) {
  // Construct query plan
  LOG_INFO("Hi from seqscan test");
  auto parser = Parser();
  parser.ParseQuery("SELECT * FROM temp;");
  // LOG_INFO("parser success: %d", parser.success);
}

}  // namespace bustub
