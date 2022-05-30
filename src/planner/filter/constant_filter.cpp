// #include "planner/filter/constant_filter.h"

// namespace bustub {

// ConstantFilter::ConstantFilter(ExpressionType comparison_type_p, Value constant_p)
//     : TableFilter(TableFilterType::CONSTANT_COMPARISON),
//       comparison_type(comparison_type_p),
//       constant(move(constant_p)) {}

// string ConstantFilter::ToString(const string &column_name) {
//   return column_name + ExpressionTypeToOperator(comparison_type) + constant.ToString();
// }

// bool ConstantFilter::Equals(const TableFilter &other_p) const {
//   if (!TableFilter::Equals(other_p)) {
//     return false;
//   }
//   auto &other = (ConstantFilter &)other_p;
//   return other.comparison_type == comparison_type && other.constant == constant;
// }

// }  // namespace bustub
