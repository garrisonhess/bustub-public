#include "execution/physical_plan_generator.h"
#include "execution/plans/abstract_plan.h"
#include "main/client_context.h"

namespace bustub {

PhysicalPlanGenerator::PhysicalPlanGenerator(ClientContext &context) : context_(context) {}

unique_ptr<AbstractPlanNode> PhysicalPlanGenerator::CreatePlan(unique_ptr<LogicalOperator> op) {
  // // first resolve column references
  // ColumnBindingResolver resolver;
  // resolver.VisitOperator(*op);

  // now resolve types of all the operators
  op->ResolveOperatorTypes();

  // then create the main physical plan
  auto plan = CreatePlan(*op);

  // plan->Verify();
  return plan;
}

unique_ptr<AbstractPlanNode> PhysicalPlanGenerator::CreatePlan(LogicalOperator &op) {
  switch (op.type_) {
    case LogicalOperatorType::LOGICAL_DUMMY_SCAN:
      return CreatePlan((LogicalDummyScan &)op);
    case LogicalOperatorType::LOGICAL_EMPTY_RESULT:
      return CreatePlan((LogicalEmptyResult &)op);
    case LogicalOperatorType::LOGICAL_GET:
    case LogicalOperatorType::LOGICAL_PROJECTION:
    case LogicalOperatorType::LOGICAL_FILTER:
    case LogicalOperatorType::LOGICAL_AGGREGATE_AND_GROUP_BY:
    case LogicalOperatorType::LOGICAL_WINDOW:
    case LogicalOperatorType::LOGICAL_UNNEST:
    case LogicalOperatorType::LOGICAL_LIMIT:
    case LogicalOperatorType::LOGICAL_LIMIT_PERCENT:
    case LogicalOperatorType::LOGICAL_SAMPLE:
    case LogicalOperatorType::LOGICAL_ORDER_BY:
    case LogicalOperatorType::LOGICAL_TOP_N:
    case LogicalOperatorType::LOGICAL_COPY_TO_FILE:
    case LogicalOperatorType::LOGICAL_ANY_JOIN:
    case LogicalOperatorType::LOGICAL_DELIM_JOIN:
    case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
    case LogicalOperatorType::LOGICAL_CROSS_PRODUCT:
    case LogicalOperatorType::LOGICAL_UNION:
    case LogicalOperatorType::LOGICAL_EXCEPT:
    case LogicalOperatorType::LOGICAL_INTERSECT:
    case LogicalOperatorType::LOGICAL_INSERT:
    case LogicalOperatorType::LOGICAL_DELETE:
    case LogicalOperatorType::LOGICAL_CHUNK_GET:
    case LogicalOperatorType::LOGICAL_DELIM_GET:
    case LogicalOperatorType::LOGICAL_EXPRESSION_GET:
    case LogicalOperatorType::LOGICAL_UPDATE:
    case LogicalOperatorType::LOGICAL_CREATE_TABLE:
    case LogicalOperatorType::LOGICAL_CREATE_INDEX:
    case LogicalOperatorType::LOGICAL_EXPLAIN:
    case LogicalOperatorType::LOGICAL_SHOW:
    case LogicalOperatorType::LOGICAL_DISTINCT:
    case LogicalOperatorType::LOGICAL_PREPARE:
    case LogicalOperatorType::LOGICAL_EXECUTE:
    case LogicalOperatorType::LOGICAL_CREATE_VIEW:
    case LogicalOperatorType::LOGICAL_CREATE_SEQUENCE:
    case LogicalOperatorType::LOGICAL_CREATE_SCHEMA:
    case LogicalOperatorType::LOGICAL_CREATE_MACRO:
    case LogicalOperatorType::LOGICAL_CREATE_TYPE:
    case LogicalOperatorType::LOGICAL_PRAGMA:
    case LogicalOperatorType::LOGICAL_TRANSACTION:
    case LogicalOperatorType::LOGICAL_ALTER:
    case LogicalOperatorType::LOGICAL_DROP:
    case LogicalOperatorType::LOGICAL_VACUUM:
    case LogicalOperatorType::LOGICAL_LOAD:
    case LogicalOperatorType::LOGICAL_RECURSIVE_CTE:
    case LogicalOperatorType::LOGICAL_CTE_REF:
    case LogicalOperatorType::LOGICAL_EXPORT:
    case LogicalOperatorType::LOGICAL_SET:
      throw NotImplementedException("Unimplemented logical operator type!");

    default:
      throw NotImplementedException("Unimplemented logical operator type!");
  }
}

}  // namespace bustub
