#include "planner/filter/conjunction_filter.h"

namespace bustub {

ConjunctionOrFilter::ConjunctionOrFilter() : ConjunctionFilter(TableFilterType::CONJUNCTION_OR) {}


string ConjunctionOrFilter::ToString(const string &column_name) {
  string result;
  for (uint64_t i = 0; i < child_filters.size(); i++) {
    if (i > 0) {
      result += " OR ";
    }
    result += child_filters[i]->ToString(column_name);
  }
  return result;
}

bool ConjunctionOrFilter::Equals(const TableFilter &other_p) const {
  if (!ConjunctionFilter::Equals(other_p)) {
    return false;
  }
  auto &other = (ConjunctionOrFilter &)other_p;
  if (other.child_filters.size() != child_filters.size()) {
    return false;
  }
  for (uint64_t i = 0; i < other.child_filters.size(); i++) {
    if (!child_filters[i]->Equals(*other.child_filters[i])) {
      return false;
    }
  }
  return true;
}

ConjunctionAndFilter::ConjunctionAndFilter() : ConjunctionFilter(TableFilterType::CONJUNCTION_AND) {}



string ConjunctionAndFilter::ToString(const string &column_name) {
  string result;
  for (uint64_t i = 0; i < child_filters.size(); i++) {
    if (i > 0) {
      result += " AND ";
    }
    result += child_filters[i]->ToString(column_name);
  }
  return result;
}

bool ConjunctionAndFilter::Equals(const TableFilter &other_p) const {
  if (!ConjunctionFilter::Equals(other_p)) {
    return false;
  }
  auto &other = (ConjunctionAndFilter &)other_p;
  if (other.child_filters.size() != child_filters.size()) {
    return false;
  }
  for (uint64_t i = 0; i < other.child_filters.size(); i++) {
    if (!child_filters[i]->Equals(*other.child_filters[i])) {
      return false;
    }
  }
  return true;
}

}  // namespace bustub
