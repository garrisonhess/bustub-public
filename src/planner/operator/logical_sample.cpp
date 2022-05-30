#include "planner/operator/logical_sample.h"

namespace bustub {

LogicalSample::LogicalSample(unique_ptr<SampleOptions> sample_options_p, unique_ptr<LogicalOperator> child)
    : LogicalOperator(LogicalOperatorType::LOGICAL_SAMPLE), sample_options(move(sample_options_p)) {
  children.push_back(move(child));
}

vector<ColumnBinding> LogicalSample::GetColumnBindings() { return children[0]->GetColumnBindings(); }

uint64_t LogicalSample::EstimateCardinality(ClientContext &context) {
  auto child_cardinality = children[0]->EstimateCardinality(context);
  if (sample_options->is_percentage) {
    return uint64_t(child_cardinality * sample_options->sample_size.GetValue<double>());
  } else {
    auto sample_size = sample_options->sample_size.GetValue<uint64_t>();
    if (sample_size < child_cardinality) {
      return sample_size;
    }
  }
  return child_cardinality;
}

void LogicalSample::ResolveTypes() { types = children[0]->types; }

}  // namespace bustub
