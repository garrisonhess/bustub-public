//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/filter/conjunction_filter.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/vector.h"
#include "planner/table_filter.h"

namespace bustub {
class ConjunctionFilter : public TableFilter {
 public:
  ConjunctionFilter(TableFilterType filter_type_p) : TableFilter(filter_type_p) {}

  virtual ~ConjunctionFilter() {}

  //! The filters of this conjunction
  vector<unique_ptr<TableFilter>> child_filters;

 public:
  virtual FilterPropagateResult CheckStatistics(BaseStatistics &stats) = 0;
  virtual string ToString(const string &column_name) = 0;

  virtual bool Equals(const TableFilter &other) const { return TableFilter::Equals(other); }
};

class ConjunctionOrFilter : public ConjunctionFilter {
 public:
  ConjunctionOrFilter();

 public:
  FilterPropagateResult CheckStatistics(BaseStatistics &stats) override;
  string ToString(const string &column_name) override;
  bool Equals(const TableFilter &other) const override;
};

class ConjunctionAndFilter : public ConjunctionFilter {
 public:
  ConjunctionAndFilter();

 public:
  FilterPropagateResult CheckStatistics(BaseStatistics &stats) override;
  string ToString(const string &column_name) override;
  bool Equals(const TableFilter &other) const override;
};

}  // namespace bustub
