#include "catalog/catalog_entry/scalar_macro_catalog_entry.h"
#include "catalog/catalog_entry/table_macro_catalog_entry.h"
#include "common/limits.h"
#include "common/string_util.h"
#include "function/table_macro_function.h"
#include "main/config.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/comparison_expression.h"
#include "parser/expression/function_expression.h"
#include "parser/expression/subquery_expression.h"
#include "parser/parsed_expression_iterator.h"
#include "parser/query_node/select_node.h"
#include "parser/tableref/joinref.h"
#include "planner/binder.h"
#include "planner/expression_binder.h"

namespace bustub {

unique_ptr<QueryNode> Binder::BindTableMacro(FunctionExpression &function, TableMacroCatalogEntry *macro_func,
                                             uint64_t depth) {
  auto &macro_def = (TableMacroFunction &)*macro_func->function;
  auto node = macro_def.query_node->Copy();

  // auto &macro_def = *macro_func->function;

  // validate the arguments and separate positional and default arguments
  vector<unique_ptr<ParsedExpression>> positionals;
  unordered_map<string, unique_ptr<ParsedExpression>> defaults;
  string error =
      MacroFunction::ValidateArguments(*macro_func->function, macro_func->name, function, positionals, defaults);
  if (!error.empty()) {
    // cannot use error below as binder rnot in scope
    // return BindResult(binder. FormatError(*expr->get(), error));
    throw BinderException(FormatError(function, error));
  }

  // create a MacroBinding to bind this macro's parameters to its arguments
  vector<LogicalType> types;
  vector<string> names;
  // positional parameters
  for (uint64_t i = 0; i < macro_def.parameters.size(); i++) {
    types.emplace_back(LogicalType::SQLNULL);
    auto &param = (ColumnRefExpression &)*macro_def.parameters[i];
    names.push_back(param.GetColumnName());
  }
  // default parameters
  for (auto it = macro_def.default_parameters.begin(); it != macro_def.default_parameters.end(); it++) {
    types.emplace_back(LogicalType::SQLNULL);
    names.push_back(it->first);
    // now push the defaults into the positionals
    positionals.push_back(move(defaults[it->first]));
  }
  auto new_macro_binding = make_unique<MacroBinding>(types, names, macro_func->name);
  new_macro_binding->arguments = move(positionals);

  // We need an EXpressionBinder So that we can call ExpressionBinder::ReplaceMacroParametersRecursive()
  auto eb = ExpressionBinder(*this, this->context);

  eb.macro_binding = new_macro_binding.get();

  /* Does it all goes throu every expression in a selectstmt  */
  ParsedExpressionIterator::EnumerateQueryNodeChildren(
      *node, [&](unique_ptr<ParsedExpression> &child) { eb.ReplaceMacroParametersRecursive(child); });

  return node;
}

}  // namespace bustub
