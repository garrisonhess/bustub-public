// #include "catalog/catalog_entry/view_catalog_entry.h"
#include "common/exception.h"
#include "common/string_util.h"
#include "main/config.h"
#include "parser/query_node/select_node.h"
#include "parser/statement/select_statement.h"
#include "parser/tableref/basetableref.h"
#include "planner/binder.h"
#include "planner/tableref/bound_basetableref.h"
#include "planner/tableref/bound_dummytableref.h"

namespace bustub {

unique_ptr<BoundTableRef> Binder::Bind(BaseTableRef &ref) {
  // // extract a table or view from the catalog
  // auto &catalog = Catalog::GetCatalog(context);
  // auto table_or_view =
  //     catalog.GetEntry(context, CatalogType::TABLE_ENTRY, ref.schema_name_, ref.table_name_, true, error_context);
  // switch (table_or_view->type) {
  //   case CatalogType::TABLE_ENTRY: {
  //     // base table: create the BoundBaseTableRef node
  //     auto table_index = GenerateTableIndex();
  //     auto table = (TableCatalogEntry *)table_or_view;

  //     auto scan_function = TableScanFunction::GetFunction();
  //     auto bind_data = make_unique<TableScanBindData>(table);
  //     auto alias = ref.alias.empty() ? ref.table_name : ref.alias;
  //     vector<Type> table_types;
  //     vector<string> table_names;
  //     for (auto &col : table->columns) {
  //       table_types.push_back(col.type);
  //       table_names.push_back(col.name);
  //     }
  //     table_names = BindContext::AliasColumnNames(alias, table_names, ref.column_name_alias);

  //     auto logical_get = make_unique<LogicalGet>(table_index, scan_function, move(bind_data), table_types,
  //     table_names); bind_context.AddBaseTable(table_index, alias, table_names, table_types, *logical_get); return
  //     make_unique_base<BoundTableRef, BoundBaseTableRef>(table, move(logical_get));
  //   }
  //   case CatalogType::VIEW_ENTRY:
  //     throw Exception("Catalog entry type");
  //   default:
  //     throw Exception("Catalog entry type");
  // }

  throw NotImplementedException("bind not impl");
}
}  // namespace bustub
