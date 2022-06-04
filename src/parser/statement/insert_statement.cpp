#include "parser/statement/insert_statement.h"
#include "common/limits.h"

namespace bustub {

Value TransformConstant(bustub_libpgquery::PGAConst *c) {
  bustub_libpgquery::PGValue pg_val = c->val;

  switch (pg_val.type) {
    case bustub_libpgquery::T_PGInteger:
      assert(pg_val.val.ival <= BUSTUB_INT32_MAX);
      return Value(TypeId::INTEGER, static_cast<int32_t>(pg_val.val.ival));
    default:
      throw NotImplementedException("Value not implemented!");
  }
}

void TransformExpressionList(bustub_libpgquery::PGList &list, vector<Value> &result) {
  for (auto node = list.head; node != nullptr; node = node->next) {
    auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
    assert(target);

    switch (target->type) {
      case bustub_libpgquery::T_PGAConst: {
        auto val = TransformConstant(reinterpret_cast<bustub_libpgquery::PGAConst *>(node));
        result.push_back(val);
        break;
      }
      default:
        throw NotImplementedException("Expr of type not implemented\n");
    }
  }
}

InsertStatement::InsertStatement(Parser &parser, bustub_libpgquery::PGInsertStmt *pg_stmt)
    : SQLStatement(StatementType::INSERT_STATEMENT) {
  table_ = pg_stmt->relation->relname;

  vector<string> col_names;
  // resolve the insertion column names
  if (pg_stmt->cols != nullptr) {
    for (auto c = pg_stmt->cols->head; c != nullptr; c = lnext(c)) {
      auto target = static_cast<bustub_libpgquery::PGResTarget *>(c->data.ptr_value);
      col_names.emplace_back(target->name);
    }
  }


  // Now resolve the column types using the catalog.


  // resolve the insertion values
  bustub_libpgquery::PGSelectStmt *select = reinterpret_cast<bustub_libpgquery::PGSelectStmt *>(pg_stmt->selectStmt);

  for (auto value_list = select->valuesLists->head; value_list != nullptr; value_list = value_list->next) {
    auto target = static_cast<bustub_libpgquery::PGList *>(value_list->data.ptr_value);
    vector<Value> curr_values;
    TransformExpressionList(*target, curr_values);

    if (!values_.empty() && values_[0].size() != curr_values.size()) {
      throw Exception("VALUES lists must all be the same length");
    }
    values_.push_back(curr_values);
  }
}

}  // namespace bustub
