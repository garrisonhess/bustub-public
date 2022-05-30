//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/operator/logical_sample.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/sample_options.h"
#include "planner/logical_operator.h"

namespace bustub {

//! LogicalSample represents a SAMPLE clause
class LogicalSample : public LogicalOperator {
 public:
  LogicalSample(unique_ptr<SampleOptions> sample_options_p, unique_ptr<LogicalOperator> child);

  //! The sample options
  unique_ptr<SampleOptions> sample_options;

 public:
  vector<ColumnBinding> GetColumnBindings() override;

  uint64_t EstimateCardinality(ClientContext &context) override;

 protected:
  void ResolveTypes() override;
};

}  // namespace bustub
