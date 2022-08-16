//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// abstract_executor.h
//
// Identification: src/include/execution/executors/abstract_executor.h
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "main/client_context.h"
#include "storage/table/tuple.h"

namespace bustub {
/**
 * The AbstractExecutor implements the Volcano tuple-at-a-time iterator model.
 * This is the base class from which all executors in the BustTub execution
 * engine inherit, and defines the minimal interface that all executors support.
 */
class AbstractExecutor {
 public:
  /**
   * Construct a new AbstractExecutor instance.
   * @param context the executor context that the executor runs with
   */
  explicit AbstractExecutor(ClientContext &context) : context_{context} {}

  /** Virtual destructor. */
  virtual ~AbstractExecutor() = default;

  /**
   * Initialize the executor.
   * @warning This function must be called before Next() is called!
   */
  virtual void Init() = 0;

  /**
   * Yield the next tuple from this executor.
   * @param[out] tuple The next tuple produced by this executor
   * @param[out] rid The next tuple RID produced by this executor
   * @return `true` if a tuple was produced, `false` if there are no more tuples
   */
  virtual auto Next(Tuple *tuple, RID *rid) -> bool = 0;

  /** @return The schema of the tuples that this executor produces */
  virtual auto GetOutputSchema() -> const Schema * = 0;

  /** @return The executor context in which this executor runs */
  auto GetExecutorContext() -> ClientContext & { return context_; }

  /** The executor context in which the executor runs */
  ClientContext &context_;
};
}  // namespace bustub
