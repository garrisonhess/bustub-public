#include "main/query_result.h"

#include <utility>
#include "common/printer.h"

namespace bustub {
using std::string;

QueryResult::QueryResult(QueryResultType type, StatementType statement_type)
    : type_(type), statement_type_(statement_type), success_(true) {}

QueryResult::QueryResult(QueryResultType type, StatementType statement_type, vector<Type> types, vector<string> names)
    : type_(type), statement_type_(statement_type), types_(move(types)), names_(move(names)), success_(true) {
  // D_ASSERT(types.size() == names.size());
}

QueryResult::QueryResult(QueryResultType type, string error) : type_(type), success_(false), error_(std::move(error)) {}

bool QueryResult::Equals(QueryResult &other) {
  // first compare the success state of the results
  if (success_ != other.success_) {
    return false;
  }
  if (!success_) {
    return error_ == other.error_;
  }
  // compare names
  if (names_ != other.names_) {
    return false;
  }
  // compare types
  if (types_ != other.types_) {
    return false;
  }
  // // now compare the actual values
  // // fetch chunks
  // while (true) {
  // 	auto lchunk = Fetch();
  // 	auto rchunk = other.Fetch();
  // 	if (lchunk->size() == 0 && rchunk->size() == 0) {
  // 		return true;
  // 	}
  // 	if (lchunk->size() != rchunk->size()) {
  // 		return false;
  // 	}
  // 	D_ASSERT(lchunk->ColumnCount() == rchunk->ColumnCount());
  // 	for (idx_t col = 0; col < rchunk->ColumnCount(); col++) {
  // 		for (idx_t row = 0; row < rchunk->size(); row++) {
  // 			auto lvalue = lchunk->GetValue(col, row);
  // 			auto rvalue = rchunk->GetValue(col, row);
  // 			if (lvalue != rvalue) {
  // 				return false;
  // 			}
  // 		}
  // 	}
  // }
}

void QueryResult::Print() { Printer::Print(ToString()); }

// string QueryResult::HeaderToString() {
// 	string result;
// 	for (auto &name : names) {
// 		result += name + "\t";
// 	}
// 	result += "\n";
// 	for (auto &type : types) {
// 		result += type.ToString() + "\t";
// 	}
// 	result += "\n";
// 	return result;
// }

// struct DuckDBArrowSchemaHolder {
// 	// unused in root
// 	ArrowSchema schema;
// 	// unused in children
// 	unique_ptr<ArrowSchema *[]> children; // just space for the *pointers* to children, not the children themselves
// };

// static void release_duckdb_arrow_schema(ArrowSchema *schema) {
// 	if (!schema || !schema->release) {
// 		return;
// 	}
// 	schema->release = nullptr;
// 	auto holder = (DuckDBArrowSchemaHolder *)schema->private_data;
// 	delete holder;
// }

// void QueryResult::ToArrowSchema(ArrowSchema *out_schema) {
// 	D_ASSERT(out_schema);

// 	auto root_holder = new DuckDBArrowSchemaHolder();

// 	root_holder->children = unique_ptr<ArrowSchema *[]>(new ArrowSchema *[ColumnCount()]);
// 	out_schema->private_data = root_holder;
// 	out_schema->release = release_duckdb_arrow_schema;

// 	out_schema->children = root_holder->children.get();

// 	out_schema->format = "+s"; // struct apparently
// 	out_schema->n_children = ColumnCount();
// 	out_schema->flags = 0;
// 	out_schema->metadata = nullptr;
// 	out_schema->name = "duckdb_query_result";
// 	out_schema->dictionary = nullptr;

// 	for (idx_t col_idx = 0; col_idx < ColumnCount(); col_idx++) {
// 		auto holder = new DuckDBArrowSchemaHolder();
// 		auto &child = holder->schema;
// 		child.private_data = holder;
// 		child.release = release_duckdb_arrow_schema;
// 		child.flags = ARROW_FLAG_NULLABLE;

// 		child.name = names[col_idx].c_str();
// 		child.n_children = 0;
// 		child.children = nullptr;
// 		child.flags = 0;
// 		child.metadata = nullptr;
// 		child.dictionary = nullptr;

// 		switch (types[col_idx].id()) {
// 		case LogicalTypeId::BOOLEAN:
// 			child.format = "b";
// 			break;
// 		case LogicalTypeId::TINYINT:
// 			child.format = "c";
// 			break;
// 		case LogicalTypeId::SMALLINT:
// 			child.format = "s";
// 			break;
// 		case LogicalTypeId::INTEGER:
// 			child.format = "i";
// 			break;
// 		case LogicalTypeId::BIGINT:
// 			child.format = "l";
// 			break;
// 		case LogicalTypeId::FLOAT:
// 			child.format = "f";
// 			break;
// 		case LogicalTypeId::HUGEINT:
// 			child.format = "d:38,0";
// 			break;
// 		case LogicalTypeId::DOUBLE:
// 			child.format = "g";
// 			break;
// 		case LogicalTypeId::VARCHAR:
// 			child.format = "u";
// 			break;
// 		case LogicalTypeId::DATE:
// 			child.format = "tdD";
// 			break;
// 		case LogicalTypeId::TIME:
// 			child.format = "ttm";
// 			break;
// 		case LogicalTypeId::TIMESTAMP:
// 			child.format = "tsn:";
// 			break;
// 		default:
// 			throw NotImplementedException("Unsupported Arrow type " + types[col_idx].ToString());
// 		}
// 		out_schema->children[col_idx] = &child;
// 	}
// }

}  // namespace bustub
