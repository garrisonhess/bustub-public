#include "parser/expression/lambda_expression.h"
#include "parser/expression/operator_expression.h"
#include "planner/expression_binder.h"

namespace bustub {

// static string ExtractColumnFromLambda(ParsedExpression &expr) {
//	if (expr.type != ExpressionType::COLUMN_REF) {
//		throw ParserException("Lambda parameter must be a column name");
//	}
//	auto &colref = (ColumnRefExpression &)expr;
//	if (colref.IsQualified()) {
//		throw ParserException("Lambda parameter must be an unqualified name (e.g. 'x', not 'a.x')");
//	}
//	return colref.column_names[0];
// }

BindResult ExpressionBinder::BindExpression(LambdaExpression &expr, uint64_t depth) {
  string error;

  // FIXME: decide from the context whether this is actually a LambdaExpression
  //  If it is, bind it as a lambda here
  // // set up the lambda capture
  // FIXME: need to get the type from the list for binding
  // lambda_capture = expr.capture_name;
  // // bind the child
  // BindChild(expr.expression, depth, error);
  // // clear the lambda capture
  // lambda_capture = "";
  // if (!error.empty()) {
  // 	return BindResult(error);
  // }
  // expr.Print();
  OperatorExpression arrow_expr(ExpressionType::ARROW, move(expr.lhs), move(expr.rhs));
  return ExpressionBinder::BindExpression(arrow_expr, depth);
}

}  // namespace bustub
