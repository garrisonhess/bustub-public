// // #include "catalog/catalog.h"
// // #include "catalog/catalog_search_path.h"
// // #include "catalog/catalog_entry/schema_catalog_entry.h"
// // #include "catalog/catalog_entry/type_catalog_entry.h"
// #include "main/client_context.h"
// #include "main/database.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/expression/subquery_expression.h"
// // #include "parser/parsed_data/create_index_info.h"
// // #include "parser/parsed_data/create_macro_info.h"
// // #include "parser/parsed_data/create_view_info.h"
// #include "parser/parsed_expression_iterator.h"
// #include "parser/statement/create_statement.h"
// #include "planner/binder.h"
// #include "planner/bound_query_node.h"
// // #include "planner/expression_binder/aggregate_binder.h"
// #include "planner/expression_binder/index_binder.h"
// #include "planner/expression_binder/select_binder.h"
// // #include "planner/operator/logical_create.h"
// #include "common/exception.h"
// #include "planner/operator/logical_create_index.h"
// #include "planner/operator/logical_create_table.h"
// #include "planner/operator/logical_get.h"
// // #include "planner/parsed_data/bound_create_function_info.h"
// // #include "planner/parsed_data/bound_create_table_info.h"
// #include "planner/query_node/bound_select_node.h"
// #include "planner/tableref/bound_basetableref.h"
// // #include "parser/constraints/foreign_key_constraint.h"
// // #include "function/scalar_macro_function.h"
// // #include "storage/data_table.h"
// // #include "main/client_data.h"
// // #include "parser/constraints/unique_constraint.h"

// namespace bustub {

// // SchemaCatalogEntry *Binder::BindSchema(CreateInfo &info) {
// // 	if (info.schema.empty()) {
// // 		info.schema = info.temporary ? TEMP_SCHEMA : ClientData::Get(context).catalog_search_path->GetDefault();
// // 	}

// // 	if (!info.temporary) {
// // 		// non-temporary create: not read only
// // 		if (info.schema == TEMP_SCHEMA) {
// // 			throw Exception("Only TEMPORARY table names can use the \"temp\" schema");
// // 		}
// // 		properties.read_only = false;
// // 	} else {
// // 		if (info.schema != TEMP_SCHEMA) {
// // 			throw Exception("TEMPORARY table names can *only* use the X schema");
// // 		}
// // 	}
// // 	// fetch the schema in which we want to create the object
// // 	auto schema_obj = Catalog::GetCatalog(context).GetSchema(context, info.schema);
// // 	assert(schema_obj->type == CatalogType::SCHEMA_ENTRY);
// // 	info.schema = schema_obj->name;
// // 	return schema_obj;
// // }

// // void Binder::BindType(ClientContext &context, Type &type, const string &schema) {
// // 	if (type.GetTypeId() == TypeId::LIST) {
// // 		auto child_type = ListType::GetChildType(type);
// // 		BindLogicalType(context, child_type, schema);
// // 		type = LogicalType::LIST(child_type);
// // 	} else if (type.id() == LogicalTypeId::STRUCT || type.id() == LogicalTypeId::MAP) {
// // 		auto child_types = StructType::GetChildTypes(type);
// // 		for (auto &child_type : child_types) {
// // 			BindLogicalType(context, child_type.second, schema);
// // 		}
// // 		// Generate new Struct/Map Type
// // 		if (type.id() == LogicalTypeId::STRUCT) {
// // 			type = LogicalType::STRUCT(child_types);
// // 		} else {
// // 			type = LogicalType::MAP(child_types);
// // 		}
// // 	} else if (type.id() == LogicalTypeId::USER) {
// // 		auto &catalog = Catalog::GetCatalog(context);
// // 		type = catalog.GetType(context, schema, UserType::GetTypeName(type));
// // 	} else if (type.id() == LogicalTypeId::ENUM) {
// // 		auto &enum_type_name = EnumType::GetTypeName(type);
// // 		auto enum_type_catalog = (TypeCatalogEntry *)context.db->GetCatalog().GetEntry(context,
// // CatalogType::TYPE_ENTRY, schema, enum_type_name, true); 		EnumType::SetCatalog(type, enum_type_catalog);
// // 	}
// // }

// static void CreateColumnMap(BoundCreateTableInfo &info, bool allow_duplicate_names) {
//   auto &base = (CreateTableInfo &)*info.base;

//   for (uint64_t oid = 0; oid < base.columns.size(); oid++) {
//     auto &col = base.columns[oid];
//     if (allow_duplicate_names) {
//       uint64_t index = 1;
//       string base_name = col.name;
//       while (info.name_map.find(col.name) != info.name_map.end()) {
//         col.name = base_name + ":" + std::to_string(index++);
//       }
//     } else {
//       if (info.name_map.find(col.name) != info.name_map.end()) {
//         throw CatalogException("Column with name %s already exists!", col.name);
//       }
//     }

//     info.name_map[col.name] = oid;
//     col.oid = oid;
//   }
// }

// unique_ptr<BoundCreateTableInfo> Binder::BindCreateTableInfo(unique_ptr<CreateInfo> info) {

//   result->schema = BindSchema(*result->base);
//   if (base.query) {
//     // construct the result object
//     auto query_obj = Bind(*base.query);
//     result->query = move(query_obj.plan);

//     // construct the set of columns based on the names and types of the query
//     auto &names = query_obj.names;
//     auto &sql_types = query_obj.types;
//     assert(names.size() == sql_types.size());
//     for (uint64_t i = 0; i < names.size(); i++) {
//       base.columns.emplace_back(names[i], sql_types[i]);
//     }
//     // create the name map for the statement
//     CreateColumnMap(*result, true);
//   } else {
//     // create the name map for the statement
//     CreateColumnMap(*result, false);
//   }
//   return result;
// }

// BoundStatement Binder::Bind(CreateStatement &stmt) {
//   BoundStatement result;
//   result.names_ = {"Count"};
//   result.types_ = {Type(TypeId::BIGINT)};

//   auto catalog_type = stmt.type_;
//   switch (catalog_type) {
//     case CatalogType::INDEX_ENTRY:
//       // {
//       //   auto &base = (CreateIndexInfo &)*stmt.info;

//       //   // visit the table reference
//       //   auto bound_table = Bind(*base.table);
//       //   if (bound_table->type != TableReferenceType::BASE_TABLE) {
//       //     throw BinderException("Can only delete from base table!");
//       //   }
//       //   auto &table_binding = (BoundBaseTableRef &)*bound_table;
//       //   auto table = table_binding.table;
//       //   // bind the index expressions
//       //   vector<unique_ptr<Expression>> expressions;
//       //   IndexBinder binder(*this, context);
//       //   for (auto &expr : base.expressions) {
//       //     expressions.push_back(binder.Bind(expr));
//       //   }

//       //   auto plan = CreatePlan(*bound_table);
//       //   if (plan->type != LogicalOperatorType::LOGICAL_GET) {
//       //     throw BinderException("Cannot create index on a view!");
//       //   }
//       //   auto &get = (LogicalGet &)*plan;
//       //   for (auto &column_id : get.column_ids) {
//       //     if (column_id == COLUMN_IDENTIFIER_ROW_ID) {
//       //       throw BinderException("Cannot create an index on the rowid!");
//       //     }
//       //   }
//       //   // this gives us a logical table scan
//       //   // we take the required columns from here
//       //   // create the logical operator
//       //   result.plan = make_unique<LogicalCreateIndex>(*table, get.column_ids, move(expressions),
//       //                                                 unique_ptr_cast<CreateInfo,
//       CreateIndexInfo>(move(stmt.info)));
//       //   break;
//       // }
//       break;
//     case CatalogType::TABLE_ENTRY: {
//       auto &create_info = (CreateTableInfo &)*stmt.info;
//       auto &catalog = Catalog::GetCatalog(context_);

//       // create the logical operator
//       auto &schema = bound_info->schema;
//       auto create_table = make_unique<LogicalCreateTable>(schema, move(bound_info));
//       result.plan_ = move(create_table);
//       break;
//     }
//     case CatalogType::SCHEMA_ENTRY:
//     case CatalogType::VIEW_ENTRY:
//     case CatalogType::SEQUENCE_ENTRY:
//     case CatalogType::TABLE_MACRO_ENTRY:
//     case CatalogType::MACRO_ENTRY:
//     case CatalogType::TYPE_ENTRY:
//       throw NotImplementedException("Unrecognized type!");
//     default:
//       throw Exception("Unrecognized type!");
//   }
//   return result;
// }

// }  // namespace bustub
