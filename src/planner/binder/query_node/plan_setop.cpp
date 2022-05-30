#include "planner/binder.h"
#include "planner/expression/bound_cast_expression.h"
#include "planner/expression/bound_columnref_expression.h"
#include "planner/operator/logical_projection.h"
#include "planner/operator/logical_set_operation.h"
#include "planner/query_node/bound_set_operation_node.h"

namespace bustub {

unique_ptr<LogicalOperator> Binder::CastLogicalOperatorToTypes(vector<Type> &source_types, vector<Type> &target_types,
                                                               unique_ptr<LogicalOperator> op) {
  assert(op);
  // first check if we even need to cast
  assert(source_types.size() == target_types.size());
  if (source_types == target_types) {
    // source and target types are equal: don't need to cast
    return op;
  }
  // otherwise add casts
  auto node = op.get();
  if (node->type == LogicalOperatorType::LOGICAL_PROJECTION) {
    // "node" is a projection; we can just do the casts in there
    assert(node->expressions.size() == source_types.size());
    // add the casts to the selection list
    for (uint64_t i = 0; i < target_types.size(); i++) {
      if (source_types[i] != target_types[i]) {
        // differing types, have to add a cast
        string alias = node->expressions[i]->alias;
        node->expressions[i] = make_unique<BoundCastExpression>(move(node->expressions[i]), target_types[i]);
        node->expressions[i]->alias = alias;
      }
    }
    return op;
  } else {
    // found a non-projection operator
    // push a new projection containing the casts

    // fetch the set of column bindings
    auto setop_columns = op->GetColumnBindings();
    assert(setop_columns.size() == source_types.size());

    // now generate the expression list
    vector<unique_ptr<Expression>> select_list;
    for (uint64_t i = 0; i < target_types.size(); i++) {
      unique_ptr<Expression> result = make_unique<BoundColumnRefExpression>(source_types[i], setop_columns[i]);
      if (source_types[i] != target_types[i]) {
        // add a cast only if the source and target types are not equivalent
        result = make_unique<BoundCastExpression>(move(result), target_types[i]);
      }
      select_list.push_back(move(result));
    }
    auto projection = make_unique<LogicalProjection>(GenerateTableIndex(), move(select_list));
    projection->children.push_back(move(op));
    return move(projection);
  }
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundSetOperationNode &node) {
  // Generate the logical plan for the left and right sides of the set operation
  node.left_binder->plan_subquery = plan_subquery;
  node.right_binder->plan_subquery = plan_subquery;

  auto left_node = node.left_binder->CreatePlan(*node.left);
  auto right_node = node.right_binder->CreatePlan(*node.right);

  // check if there are any unplanned subqueries left in either child
  has_unplanned_subqueries = node.left_binder->has_unplanned_subqueries || node.right_binder->has_unplanned_subqueries;

  // for both the left and right sides, cast them to the same types
  left_node = CastLogicalOperatorToTypes(node.left->types, node.types, move(left_node));
  right_node = CastLogicalOperatorToTypes(node.right->types, node.types, move(right_node));

  // create actual logical ops for setops
  LogicalOperatorType logical_type;
  switch (node.setop_type) {
    case SetOperationType::UNION:
      logical_type = LogicalOperatorType::LOGICAL_UNION;
      break;
    case SetOperationType::EXCEPT:
      logical_type = LogicalOperatorType::LOGICAL_EXCEPT;
      break;
    default:
      assert(node.setop_type == SetOperationType::INTERSECT);
      logical_type = LogicalOperatorType::LOGICAL_INTERSECT;
      break;
  }
  auto root = make_unique<LogicalSetOperation>(node.setop_index, node.types.size(), move(left_node), move(right_node),
                                               logical_type);

  return VisitQueryNode(node, move(root));
}

}  // namespace bustub
