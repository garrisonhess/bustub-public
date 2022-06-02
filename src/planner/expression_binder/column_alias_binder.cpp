#include "planner/expression_binder/column_alias_binder.h"

#include "common/string_util.h"
#include "parser/expression/columnref_expression.h"
#include "planner/query_node/bound_select_node.h"

namespace bustub {

ColumnAliasBinder::ColumnAliasBinder(BoundSelectNode &node, const case_insensitive_map_t<uint64_t> &alias_map)
    : node_(node), alias_map_(alias_map), in_alias_(false) {}

BindResult ColumnAliasBinder::BindAlias(ExpressionBinder &enclosing_binder, ColumnRefExpression &expr, uint64_t depth,
                                        bool root_expression) {
  if (expr.IsQualified()) {
    return BindResult(StringUtil::Format("Alias %s cannot be qualified.", expr.ToString().c_str()));
  }

  auto alias_entry = alias_map_.find(expr.column_names_[0]);
  if (alias_entry == alias_map_.end()) {
    return BindResult(StringUtil::Format("Alias %s is not found.", expr.ToString().c_str()));
  }

  // found an alias: bind the alias expression
  assert(!in_alias_);
  auto expression = node_.original_expressions_[alias_entry->second]->Copy();
  in_alias_ = true;
  auto result = enclosing_binder.BindExpression(&expression, depth, root_expression);
  in_alias_ = false;
  return result;
}

}  // namespace bustub