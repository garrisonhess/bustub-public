#include "planner/logical_operator.h"

#include "common/exception.h"
#include "common/printer.h"
#include "common/string_util.h"
#include "parser/parser.h"

namespace bustub {

string LogicalOperator::GetName() const { return LogicalOperatorToString(type); }

string LogicalOperator::ParamsToString() const {
  string result;
  for (uint64_t i = 0; i < expressions.size(); i++) {
    if (i > 0) {
      result += "\n";
    }
    result += expressions[i]->GetName();
  }
  return result;
}

void LogicalOperator::ResolveOperatorTypes() {
  // if (types.size() > 0) {
  // 	// types already resolved for this node
  // 	return;
  // }
  types.clear();
  // first resolve child types
  for (auto &child : children) {
    child->ResolveOperatorTypes();
  }
  // now resolve the types for this operator
  ResolveTypes();
  assert(types.size() == GetColumnBindings().size());
}

vector<ColumnBinding> LogicalOperator::GenerateColumnBindings(uint64_t table_idx, uint64_t column_count) {
  vector<ColumnBinding> result;
  for (uint64_t i = 0; i < column_count; i++) {
    result.emplace_back(table_idx, i);
  }
  return result;
}

vector<Type> LogicalOperator::MapTypes(const vector<Type> &types, const vector<uint64_t> &projection_map) {
  if (projection_map.empty()) {
    return types;
  } else {
    vector<Type> result_types;
    result_types.reserve(projection_map.size());
    for (auto index : projection_map) {
      result_types.push_back(types[index]);
    }
    return result_types;
  }
}

vector<ColumnBinding> LogicalOperator::MapBindings(const vector<ColumnBinding> &bindings,
                                                   const vector<uint64_t> &projection_map) {
  if (projection_map.empty()) {
    return bindings;
  }
  vector<ColumnBinding> result_bindings;
  result_bindings.reserve(projection_map.size());
  for (auto index : projection_map) {
    result_bindings.push_back(bindings[index]);
  }
  return result_bindings;
}

string LogicalOperator::ToString() const {
  throw NotImplementedException("");
  // TreeRenderer renderer;
  // return renderer.ToString(*this);
}

void LogicalOperator::Verify() {
#ifdef DEBUG
  // verify expressions
  for (uint64_t expr_idx = 0; expr_idx < expressions.size(); expr_idx++) {
    auto str = expressions[expr_idx]->ToString();
    // verify that we can (correctly) copy this expression
    auto copy = expressions[expr_idx]->Copy();
    auto original_hash = expressions[expr_idx]->Hash();
    auto copy_hash = copy->Hash();
    // copy should be identical to original
    assert(expressions[expr_idx]->ToString() == copy->ToString());
    assert(original_hash == copy_hash);
    assert(Expression::Equals(expressions[expr_idx].get(), copy.get()));

    assert(!Expression::Equals(expressions[expr_idx].get(), nullptr));
    for (uint64_t other_idx = 0; other_idx < expr_idx; other_idx++) {
      // comparison with other expressions
      auto other_hash = expressions[other_idx]->Hash();
      bool expr_equal = Expression::Equals(expressions[expr_idx].get(), expressions[other_idx].get());
      if (original_hash != other_hash) {
        // if the hashes are not equal the expressions should not be equal either
        assert(!expr_equal);
      }
    }
    assert(!str.empty());
  }
  assert(!ToString().empty());
  for (auto &child : children) {
    child->Verify();
  }
#endif
}

void LogicalOperator::Print() { Printer::Print(ToString()); }

}  // namespace bustub
