

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

#include "buffer/buffer_pool_manager_instance.h"
#include "concurrency/transaction_manager.h"
#include "execution/execution_engine.h"
#include "execution/executor_context.h"
#include "execution/executors/aggregation_executor.h"
#include "execution/executors/insert_executor.h"
#include "execution/executors/nested_loop_join_executor.h"
#include "execution/expressions/aggregate_value_expression.h"
#include "execution/expressions/column_value_expression.h"
#include "execution/expressions/comparison_expression.h"
#include "execution/expressions/constant_value_expression.h"
#include "execution/plans/delete_plan.h"
#include "execution/plans/distinct_plan.h"
#include "execution/plans/hash_join_plan.h"
#include "execution/plans/limit_plan.h"
#include "execution/plans/seq_scan_plan.h"
#include "execution/plans/update_plan.h"
#include "executor_test_util.h"  // NOLINT
#include "gtest/gtest.h"
#include "storage/table/tuple.h"
#include "test_util.h"  // NOLINT
#include "type/value_factory.h"


namespace bustub {

// Parameters for index construction
using KeyType = GenericKey<8>;
using ValueType = RID;
using ComparatorType = GenericComparator<8>;
using HashFunctionType = HashFunction<KeyType>;


// SELECT col_a, col_b FROM test_1 WHERE col_a < 500
TEST_F(ExecutorTest, SimpleSeqScanTest100) {
  // Construct query plan
  LOG_INFO("Hi from seqscan test");
  auto parser = PostgresParser();
  parser.Parse("SELECT * FROM temp;");
  LOG_INFO("parser success: %d", parser.success);

  // 	bool success;
  // 	bustub_libpgquery::PGList *parse_tree;
  // 	std::string error_message;
  // 	int error_location;
  // 	void Parse(const std::string &query);
  // 	static std::vector<bustub_libpgquery::PGSimplifiedToken> Tokenize(const std::string &query);
  // 	static std::vector<bustub_libpgquery::PGKeyword> KeywordList();
}


}  // namespace bustub



