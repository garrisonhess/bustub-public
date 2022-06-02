#include "common/exception.h"
#include "parser/query_node.h"
#include "planner/binder.h"
#include "planner/bound_query_node.h"
#include "planner/operator/logical_distinct.h"
#include "planner/operator/logical_limit.h"
#include "planner/operator/logical_order.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::VisitQueryNode(BoundQueryNode &node, unique_ptr<LogicalOperator> root) {
  assert(root);
  for (auto &mod : node.modifiers_) {
    switch (mod->type_) {
      case ResultModifierType::DISTINCT_MODIFIER: {
        auto &bound = (BoundDistinctModifier &)*mod;
        auto distinct = make_unique<LogicalDistinct>(move(bound.target_distincts_));
        distinct->AddChild(move(root));
        root = move(distinct);
        break;
      }
      case ResultModifierType::ORDER_MODIFIER: {
        auto &bound = (BoundOrderModifier &)*mod;
        auto order = make_unique<LogicalOrder>(move(bound.orders_));
        order->AddChild(move(root));
        root = move(order);
        break;
      }
      case ResultModifierType::LIMIT_MODIFIER: {
        auto &bound = (BoundLimitModifier &)*mod;
        auto limit =
            make_unique<LogicalLimit>(bound.limit_val_, bound.offset_val_, move(bound.limit_), move(bound.offset_));
        limit->AddChild(move(root));
        root = move(limit);
        break;
      }
      default:
        throw Exception("Unimplemented modifier type!");
    }
  }
  return root;
}

}  // namespace bustub
