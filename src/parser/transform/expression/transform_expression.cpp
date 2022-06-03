#include "common/exception.h"
// #include "parser/expression/default_expression.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformResTarget(bustub_libpgquery::PGResTarget *root) {
  assert(root);
  auto expr = TransformExpression(root->val);
  if (!expr) {
    return nullptr;
  }
  if (root->name != nullptr) {
    expr->alias_ = std::string(root->name);
  }
  return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformNamedArg(bustub_libpgquery::PGNamedArgExpr *root) {
  assert(root);
  auto expr = TransformExpression(reinterpret_cast<bustub_libpgquery::PGNode *>(root->arg));
  if (root->name != nullptr) {
    expr->alias_ = std::string(root->name);
  }
  return expr;
}

unique_ptr<ParsedExpression> Transformer::TransformExpression(bustub_libpgquery::PGNode *node) {
  if (node == nullptr) {
    return nullptr;
  }

  switch (node->type) {
    case bustub_libpgquery::T_PGColumnRef:
      return TransformColumnRef(reinterpret_cast<bustub_libpgquery::PGColumnRef *>(node));
    case bustub_libpgquery::T_PGAConst:
      return TransformConstant(reinterpret_cast<bustub_libpgquery::PGAConst *>(node));
    case bustub_libpgquery::T_PGAStar:
      return TransformStarExpression(node);
    case bustub_libpgquery::T_PGAExpr:
      throw NotImplementedException("Expr of type T_PGAExpr not implemented\n");
    //   return TransformAExpr(reinterpret_cast<bustub_libpgquery::PGAExpr *>(node));
    case bustub_libpgquery::T_PGFuncCall:
      throw NotImplementedException("Expr of type T_PGFuncCall not implemented\n");
    //   return TransformFuncCall(reinterpret_cast<bustub_libpgquery::PGFuncCall *>(node));
    case bustub_libpgquery::T_PGBoolExpr:
      throw NotImplementedException("Expr of type T_PGBoolExpr not implemented\n");
    //   return TransformBoolExpr(reinterpret_cast<bustub_libpgquery::PGBoolExpr *>(node));
    case bustub_libpgquery::T_PGTypeCast:
      throw NotImplementedException("Expr of type T_PGTypeCast not implemented\n");
      //   return TransformTypeCast(reinterpret_cast<bustub_libpgquery::PGTypeCast *>(node));
    case bustub_libpgquery::T_PGParamRef:
      throw NotImplementedException("Expr of type T_PGParamRef not implemented\n");
      //   return TransformParamRef(reinterpret_cast<bustub_libpgquery::PGParamRef *>(node));
    case bustub_libpgquery::T_PGResTarget:
      return TransformResTarget(reinterpret_cast<bustub_libpgquery::PGResTarget *>(node));
    case bustub_libpgquery::T_PGCaseExpr:
      throw NotImplementedException("Expr of type T_PGCaseExpr not implemented\n");
    //   return TransformCase(reinterpret_cast<bustub_libpgquery::PGCaseExpr *>(node));
    case bustub_libpgquery::T_PGSubLink:
      throw NotImplementedException("Expr of type T_PGSubLink not implemented\n");
    //   return TransformSubquery(reinterpret_cast<bustub_libpgquery::PGSubLink *>(node));
    case bustub_libpgquery::T_PGCoalesceExpr:
      throw NotImplementedException("Expr of type T_PGCoalesceExpr not implemented\n");
    //   return TransformCoalesce(reinterpret_cast<bustub_libpgquery::PGAExpr *>(node));
    case bustub_libpgquery::T_PGNullTest:
      throw NotImplementedException("Expr of type T_PGNullTest not implemented\n");
    //   return TransformNullTest(reinterpret_cast<bustub_libpgquery::PGNullTest *>(node));
    case bustub_libpgquery::T_PGNamedArgExpr:
      throw NotImplementedException("Expr of type T_PGNamedArgExpr not implemented\n");
    //   return TransformNamedArg(reinterpret_cast<bustub_libpgquery::PGNamedArgExpr *>(node));
    case bustub_libpgquery::T_PGSQLValueFunction:
      throw NotImplementedException("Expr of type T_PGSQLValueFunction not implemented\n");
    //   return TransformSQLValueFunction(reinterpret_cast<bustub_libpgquery::PGSQLValueFunction *>(node));
    case bustub_libpgquery::T_PGSetToDefault:
      throw NotImplementedException("Expr of type T_PGSetToDefault not implemented\n");
    //   return std::make_unique<DefaultExpression>();
    case bustub_libpgquery::T_PGCollateClause:
      throw NotImplementedException("Expr of type T_PGCollateClause not implemented\n");
    //   return TransformCollateExpr(reinterpret_cast<bustub_libpgquery::PGCollateClause *>(node));
    case bustub_libpgquery::T_PGIntervalConstant:
      throw NotImplementedException("Expr of type T_PGIntervalConstant not implemented\n");
    //   return TransformInterval(reinterpret_cast<bustub_libpgquery::PGIntervalConstant *>(node));
    case bustub_libpgquery::T_PGLambdaFunction:
      throw NotImplementedException("Expr of type T_PGLambdaFunction not implemented\n");
    //   return TransformLambda(reinterpret_cast<bustub_libpgquery::PGLambdaFunction *>(node));
    case bustub_libpgquery::T_PGAIndirection:
      throw NotImplementedException("Expr of type T_PGAIndirection not implemented\n");
    //   return TransformArrayAccess(reinterpret_cast<bustub_libpgquery::PGAIndirection *>(node));
    case bustub_libpgquery::T_PGPositionalReference:
      throw NotImplementedException("Expr of type T_PGPositionalReference not implemented\n");
    //   return TransformPositionalReference(reinterpret_cast<bustub_libpgquery::PGPositionalReference *>(node));
    case bustub_libpgquery::T_PGGroupingFunc:
      throw NotImplementedException("Expr of type T_PGGroupingFunc not implemented\n");
    //   return TransformGroupingFunction(reinterpret_cast<bustub_libpgquery::PGGroupingFunc *>(node));
    default:
      throw NotImplementedException("Expr of type not implemented\n");
  }
}

void Transformer::TransformExpressionList(bustub_libpgquery::PGList &list,
                                          vector<unique_ptr<ParsedExpression>> &result) {
  for (auto node = list.head; node != nullptr; node = node->next) {
    auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
    assert(target);

    auto expr = TransformExpression(target);
    assert(expr);

    result.push_back(move(expr));
  }
}

}  // namespace bustub
