// #include "catalog/catalog_entry/type_catalog_entry.h"
#include "catalog/dependency_manager.h"
#include "parser/expression/cast_expression.h"
#include "planner/binder.h"
#include "planner/expression/bound_constant_expression.h"
#include "planner/expression_binder/check_binder.h"
#include "planner/expression_binder/constant_binder.h"
// #include "planner/parsed_data/bound_create_table_info.h"

#include <algorithm>

namespace bustub {

static void CreateColumnMap(BoundCreateTableInfo &info, bool allow_duplicate_names) {
  auto &base = (CreateTableInfo &)*info.base;

  for (uint64_t oid = 0; oid < base.columns.size(); oid++) {
    auto &col = base.columns[oid];
    if (allow_duplicate_names) {
      uint64_t index = 1;
      string base_name = col.name;
      while (info.name_map.find(col.name) != info.name_map.end()) {
        col.name = base_name + ":" + to_string(index++);
      }
    } else {
      if (info.name_map.find(col.name) != info.name_map.end()) {
        throw CatalogException("Column with name %s already exists!", col.name);
      }
    }

    info.name_map[col.name] = oid;
    col.oid = oid;
  }
}

void Binder::BindDefaultValues(vector<ColumnDefinition> &columns, vector<unique_ptr<Expression>> &bound_defaults) {
  for (uint64_t i = 0; i < columns.size(); i++) {
    unique_ptr<Expression> bound_default;
    if (columns[i].default_value) {
      // we bind a copy of the DEFAULT value because binding is destructive
      // and we want to keep the original expression around for serialization
      auto default_copy = columns[i].default_value->Copy();
      ConstantBinder default_binder(*this, context, "DEFAULT value");
      default_binder.target_type = columns[i].type;
      bound_default = default_binder.Bind(default_copy);
    } else {
      // no default value specified: push a default value of constant null
      bound_default = make_unique<BoundConstantExpression>(Value(columns[i].type));
    }
    bound_defaults.push_back(move(bound_default));
  }
}

unique_ptr<BoundCreateTableInfo> Binder::BindCreateTableInfo(unique_ptr<CreateInfo> info) {
  auto &base = (CreateTableInfo &)*info;

  auto result = make_unique<BoundCreateTableInfo>(move(info));
  result->schema = BindSchema(*result->base);
  if (base.query) {
    // construct the result object
    auto query_obj = Bind(*base.query);
    result->query = move(query_obj.plan);

    // construct the set of columns based on the names and types of the query
    auto &names = query_obj.names;
    auto &sql_types = query_obj.types;
    assert(names.size() == sql_types.size());
    for (uint64_t i = 0; i < names.size(); i++) {
      base.columns.emplace_back(names[i], sql_types[i]);
    }
    // create the name map for the statement
    CreateColumnMap(*result, true);
  } else {
    // create the name map for the statement
    CreateColumnMap(*result, false);
    // bind the default values
    BindDefaultValues(base.columns, result->bound_defaults);
  }
  // bind collations to detect any unsupported collation errors
  for (auto &column : base.columns) {
    ExpressionBinder::TestCollation(context, StringType::GetCollation(column.type));
    BindType(context, column.type);
    if (column.type.id() == TypeId::ENUM) {
      // We add a catalog dependency
      auto enum_dependency = EnumType::GetCatalog(column.type);
      if (enum_dependency) {
        // Only if the ENUM comes from a create type
        result->dependencies.insert(enum_dependency);
      }
    }
  }
  properties.allow_stream_result = false;
  return result;
}

}  // namespace bustub
