//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hash_join_executor.cpp
//
// Identification: src/execution/hash_join_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "execution/executors/hash_join_executor.h"
#include "main/client_context.h"

namespace bustub {

HashJoinExecutor::HashJoinExecutor(ClientContext &context, const HashJoinPlanNode *plan,
                                   std::unique_ptr<AbstractExecutor> &&left_child,
                                   std::unique_ptr<AbstractExecutor> &&right_child)
    : AbstractExecutor(context) {}

void HashJoinExecutor::Init() {}

auto HashJoinExecutor::Next(Tuple *tuple, RID *rid) -> bool { return false; }

}  // namespace bustub
