#include "main/prepared_statement_data.h"
// #include "execution/physical_operator.h"
#include "common/exception.h"
#include "parser/sql_statement.h"

namespace bustub {

PreparedStatementData::PreparedStatementData(StatementType type) : statement_type_(type) {}

PreparedStatementData::~PreparedStatementData() = default;

void PreparedStatementData::Bind(vector<Value> values) {
//   // set parameters
//   const auto required = unbound_statement ? unbound_statement->n_param : 0;
//   if (values.size() != required) {
//     throw BinderException("Parameter/argument count mismatch for prepared statement. Expected %llu, got %llu", required,
//                           values.size());
//   }

//   // bind the required values
//   for (auto &it : value_map) {
//     const int64_t i = it.first - 1;
//     if (i >= values.size()) {
//       throw Exception("Could not find parameter with index %llu", i + 1);
//     }
//     // D_ASSERT(!it.second.empty());
//     if (!values[i].TryCastAs(it.second[0]->type())) {
//       throw Exception("Type mismatch for binding parameter with index %llu, expected type %s but got type %s", i + 1,
//                       it.second[0]->type().ToString().c_str(), values[i].type().ToString().c_str());
//     }
//     for (auto &target : it.second) {
//       *target = values[i];
//     }
//   }
}

// Type PreparedStatementData::GetType(int64_t param_idx) {
//   auto it = value_map.find(param_idx);
//   if (it == value_map.end()) {
//     throw Exception("Could not find parameter with index %llu", param_idx);
//   }
//   // D_ASSERT(!it->second.empty());
//   return it->second[0]->type();
// }

}  // namespace bustub
