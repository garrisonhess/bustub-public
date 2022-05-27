#include "common/exception.h"
#include "parser/expression/default_expression.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformResTarget(bustub_libpgquery::PGResTarget *root) {
	D_ASSERT(root);

	auto expr = TransformExpression(root->val);
	if (!expr) {
		return nullptr;
	}
	if (root->name) {
		expr->alias = string(root->name);
	}
	return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformNamedArg(bustub_libpgquery::PGNamedArgExpr *root) {
	D_ASSERT(root);

	auto expr = TransformExpression((bustub_libpgquery::PGNode *)root->arg);
	if (root->name) {
		expr->alias = string(root->name);
	}
	return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformExpression(bustub_libpgquery::PGNode *node) {
	if (!node) {
		return nullptr;
	}

	auto stack_checker = StackCheck();

	switch (node->type) {
	case bustub_libpgquery::T_PGColumnRef:
		return TransformColumnRef(reinterpret_cast<bustub_libpgquery::PGColumnRef *>(node));
	case bustub_libpgquery::T_PGAConst:
		return TransformConstant(reinterpret_cast<bustub_libpgquery::PGAConst *>(node));
	case bustub_libpgquery::T_PGAExpr:
		return TransformAExpr(reinterpret_cast<bustub_libpgquery::PGAExpr *>(node));
	case bustub_libpgquery::T_PGFuncCall:
		return TransformFuncCall(reinterpret_cast<bustub_libpgquery::PGFuncCall *>(node));
	case bustub_libpgquery::T_PGBoolExpr:
		return TransformBoolExpr(reinterpret_cast<bustub_libpgquery::PGBoolExpr *>(node));
	case bustub_libpgquery::T_PGTypeCast:
		return TransformTypeCast(reinterpret_cast<bustub_libpgquery::PGTypeCast *>(node));
	case bustub_libpgquery::T_PGCaseExpr:
		return TransformCase(reinterpret_cast<bustub_libpgquery::PGCaseExpr *>(node));
	case bustub_libpgquery::T_PGSubLink:
		return TransformSubquery(reinterpret_cast<bustub_libpgquery::PGSubLink *>(node));
	case bustub_libpgquery::T_PGCoalesceExpr:
		return TransformCoalesce(reinterpret_cast<bustub_libpgquery::PGAExpr *>(node));
	case bustub_libpgquery::T_PGNullTest:
		return TransformNullTest(reinterpret_cast<bustub_libpgquery::PGNullTest *>(node));
	case bustub_libpgquery::T_PGResTarget:
		return TransformResTarget(reinterpret_cast<bustub_libpgquery::PGResTarget *>(node));
	case bustub_libpgquery::T_PGParamRef:
		return TransformParamRef(reinterpret_cast<bustub_libpgquery::PGParamRef *>(node));
	case bustub_libpgquery::T_PGNamedArgExpr:
		return TransformNamedArg(reinterpret_cast<bustub_libpgquery::PGNamedArgExpr *>(node));
	case bustub_libpgquery::T_PGSQLValueFunction:
		return TransformSQLValueFunction(reinterpret_cast<bustub_libpgquery::PGSQLValueFunction *>(node));
	case bustub_libpgquery::T_PGSetToDefault:
		return make_unique<DefaultExpression>();
	case bustub_libpgquery::T_PGCollateClause:
		return TransformCollateExpr(reinterpret_cast<bustub_libpgquery::PGCollateClause *>(node));
	case bustub_libpgquery::T_PGIntervalConstant:
		return TransformInterval(reinterpret_cast<bustub_libpgquery::PGIntervalConstant *>(node));
	case bustub_libpgquery::T_PGLambdaFunction:
		return TransformLambda(reinterpret_cast<bustub_libpgquery::PGLambdaFunction *>(node));
	case bustub_libpgquery::T_PGAIndirection:
		return TransformArrayAccess(reinterpret_cast<bustub_libpgquery::PGAIndirection *>(node));
	case bustub_libpgquery::T_PGPositionalReference:
		return TransformPositionalReference(reinterpret_cast<bustub_libpgquery::PGPositionalReference *>(node));
	case bustub_libpgquery::T_PGGroupingFunc:
		return TransformGroupingFunction(reinterpret_cast<bustub_libpgquery::PGGroupingFunc *>(node));
	case bustub_libpgquery::T_PGAStar:
		return TransformStarExpression(node);
	default:
		throw NotImplementedException("Expr of type %d not implemented\n", (int)node->type);
	}
}

void Transformer::TransformExpressionList(bustub_libpgquery::PGList &list,
                                          vector<unique_ptr<ParsedExpression>> &result) {
	for (auto node = list.head; node != nullptr; node = node->next) {
		auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
		D_ASSERT(target);

		auto expr = TransformExpression(target);
		D_ASSERT(expr);

		result.push_back(move(expr));
	}
}

} // namespace bustub
