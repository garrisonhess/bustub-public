#include "common/exception.h"
#include "parser/expression/star_expression.h"
#include "parser/query_node/select_node.h"
#include "parser/statement/select_statement.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<QueryNode> Transformer::TransformSelectNode(bustub_libpgquery::PGSelectStmt *stmt) {
  assert(stmt->type == bustub_libpgquery::T_PGSelectStmt);
  unique_ptr<QueryNode> node;

  switch (stmt->op) {
    case bustub_libpgquery::PG_SETOP_NONE: {
      node = std::make_unique<SelectNode>();
      auto result = (SelectNode *)node.get();

      // checks distinct clause
      if (stmt->distinctClause != nullptr) {
        auto modifier = std::make_unique<DistinctModifier>();
        // checks distinct on clause
        auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(stmt->distinctClause->head->data.ptr_value);
        if (target != nullptr) {
          //  add the columns defined in the ON clause to the select list
          TransformExpressionList(*stmt->distinctClause, modifier->distinct_on_targets_);
        }
        result->modifiers_.push_back(move(modifier));
      }

      // do this early so the value lists also have a `FROM`
      if (stmt->valuesLists != nullptr) {
        // VALUES list, create an ExpressionList
        // assert(!stmt->fromClause);
        result->from_table_ = TransformValuesList(stmt->valuesLists);
        result->select_list_.push_back(std::make_unique<StarExpression>());
      } else {
        if (stmt->targetList == nullptr) {
          throw Exception("SELECT clause without selection list");
        }
        // select list
        TransformExpressionList(*stmt->targetList, result->select_list_);
        result->from_table_ = TransformFrom(stmt->fromClause);
      }

      // where
      result->where_clause_ = TransformExpression(stmt->whereClause);
      // // group by
      // TransformGroupBy(stmt->groupClause, *result);
      // having
      result->having_ = TransformExpression(stmt->havingClause);
      // qualify
      result->qualify_ = TransformExpression(stmt->qualifyClause);
      break;
    }
    case bustub_libpgquery::PG_SETOP_UNION:
    case bustub_libpgquery::PG_SETOP_EXCEPT:
    case bustub_libpgquery::PG_SETOP_INTERSECT:
    default:
      throw NotImplementedException("Statement type not implemented!");
  }

  // transform the common properties
  // both the set operations and the regular select can have an ORDER BY/LIMIT attached to them
  vector<OrderByNode> orders;
  TransformOrderBy(stmt->sortClause, orders);
  if (!orders.empty()) {
    auto order_modifier = make_unique<OrderModifier>();
    order_modifier->orders_ = move(orders);
    node->modifiers_.push_back(move(order_modifier));
  }
  if ((stmt->limitCount != nullptr) || (stmt->limitOffset != nullptr)) {
    auto limit_modifier = make_unique<LimitModifier>();
    if (stmt->limitCount != nullptr) {
      limit_modifier->limit_ = TransformExpression(stmt->limitCount);
    }
    if (stmt->limitOffset != nullptr) {
      limit_modifier->offset_ = TransformExpression(stmt->limitOffset);
    }
    node->modifiers_.push_back(move(limit_modifier));
  }
  return node;
}

}  // namespace bustub
