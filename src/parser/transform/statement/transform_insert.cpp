#include "parser/statement/insert_statement.h"
#include "parser/tableref/expressionlistref.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<TableRef> Transformer::TransformValuesList(bustub_libpgquery::PGList *list) {
  auto result = std::make_unique<ExpressionListRef>();
  for (auto value_list = list->head; value_list != nullptr; value_list = value_list->next) {
    auto target = static_cast<bustub_libpgquery::PGList *>(value_list->data.ptr_value);

    vector<unique_ptr<ParsedExpression>> insert_values;
    TransformExpressionList(*target, insert_values);
    if (!result->values.empty()) {
      if (result->values[0].size() != insert_values.size()) {
        throw ParserException("VALUES lists must all be the same length");
      }
    }
    result->values.push_back(move(insert_values));
  }
  result->alias = "valueslist";
  return result;
}

unique_ptr<InsertStatement> Transformer::TransformInsert(bustub_libpgquery::PGNode *node) {
  auto stmt = reinterpret_cast<bustub_libpgquery::PGInsertStmt *>(node);
  assert(stmt);
  if ((stmt->onConflictClause != nullptr) && stmt->onConflictClause->action != bustub_libpgquery::PG_ONCONFLICT_NONE) {
    throw Exception("ON CONFLICT IGNORE/UPDATE clauses are not supported");
  }

  auto result = std::make_unique<InsertStatement>();

  // first check if there are any columns specified
  if (stmt->cols != nullptr) {
    for (auto c = stmt->cols->head; c != nullptr; c = lnext(c)) {
      auto target = static_cast<bustub_libpgquery::PGResTarget *>(c->data.ptr_value);
      result->columns_.emplace_back(target->name);
    }
  }

  // Grab and transform the returning columns from the parser.
  if (stmt->returningList != nullptr) {
    Transformer::TransformExpressionList(*(stmt->returningList), result->returning_list_);
  }
  result->select_statement_ = TransformSelect(stmt->selectStmt, false);

  auto qname = TransformQualifiedName(stmt->relation);
  result->table_ = qname.name;
  result->schema_ = qname.schema;
  return result;
}

}  // namespace bustub
