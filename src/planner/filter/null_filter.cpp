#include "planner/filter/null_filter.h"

namespace bustub {

IsNullFilter::IsNullFilter() : TableFilter(TableFilterType::IS_NULL) {}

string IsNullFilter::ToString(const string &column_name) { return column_name + "IS NULL"; }

IsNotNullFilter::IsNotNullFilter() : TableFilter(TableFilterType::IS_NOT_NULL) {}


string IsNotNullFilter::ToString(const string &column_name) { return column_name + " IS NOT NULL"; }

}  // namespace bustub
