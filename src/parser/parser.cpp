

#include "parser/parser.h"
#include <iostream>
#include <unordered_set>
#include "common/exception.h"
#include "common/logger.h"
#include "common/string_util.h"
#include "parser/sql_statement.h"
#include "parser/statement/create_statement.h"
#include "parser/statement/delete_statement.h"
#include "parser/statement/insert_statement.h"
#include "parser/statement/select_statement.h"
#include "type/decimal_type.h"

namespace bustub {
using bustub_libpgquery::PGCreateStmt;
using bustub_libpgquery::PGDeleteStmt;
using bustub_libpgquery::PGInsertStmt;
using bustub_libpgquery::PGKeywordCategory;
using bustub_libpgquery::PGSelectStmt;
using bustub_libpgquery::PGSimplifiedTokenType;
using std::move;

void Parser::ParseQuery(const std::string &query) {
  PostgresParser parser;
  parser.Parse(query);
  if (!parser.success) {
    LOG_INFO("Query failed to parse!");
    throw Exception("Query failed to parse!");
    return;
  }

  if (parser.parse_tree == nullptr) {
    LOG_INFO("parser received empty statement");
    return;
  }

  // if it succeeded, we transform the Postgres parse tree into a list of
  // SQLStatements
  TransformParseTree(parser.parse_tree, statements_);
  if (!statements_.empty()) {
    auto &last_statement = statements_.back();
    last_statement->stmt_length_ = query.size() - last_statement->stmt_location_;
    for (auto &statement : statements_) {
      statement->query_ = query;
    }
  }
}

bool Parser::IsKeyword(const std::string &text) { return PostgresParser::IsKeyword(text); }

std::vector<ParserKeyword> Parser::KeywordList() {
  auto keywords = PostgresParser::KeywordList();
  std::vector<ParserKeyword> result;
  for (auto &kw : keywords) {
    ParserKeyword res;
    res.name_ = kw.text;
    switch (kw.category) {
      case bustub_libpgquery::PGKeywordCategory::PG_KEYWORD_RESERVED:
        res.category_ = KeywordCategory::KEYWORD_RESERVED;
        break;
      case bustub_libpgquery::PGKeywordCategory::PG_KEYWORD_UNRESERVED:
        res.category_ = KeywordCategory::KEYWORD_UNRESERVED;
        break;
      case bustub_libpgquery::PGKeywordCategory::PG_KEYWORD_TYPE_FUNC:
        res.category_ = KeywordCategory::KEYWORD_TYPE_FUNC;
        break;
      case bustub_libpgquery::PGKeywordCategory::PG_KEYWORD_COL_NAME:
        res.category_ = KeywordCategory::KEYWORD_COL_NAME;
        break;
      default:
        throw Exception("Unrecognized keyword category");
    }
    result.push_back(res);
  }
  return result;
}

std::vector<SimplifiedToken> Parser::Tokenize(const std::string &query) {
  auto pg_tokens = PostgresParser::Tokenize(query);
  std::vector<SimplifiedToken> result;
  result.reserve(pg_tokens.size());
  for (auto &pg_token : pg_tokens) {
    SimplifiedToken token;
    switch (pg_token.type) {
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_IDENTIFIER:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_IDENTIFIER;
        break;
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_NUMERIC_CONSTANT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_NUMERIC_CONSTANT;
        break;
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_STRING_CONSTANT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_STRING_CONSTANT;
        break;
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_OPERATOR:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_OPERATOR;
        break;
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_KEYWORD:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_KEYWORD;
        break;
      // comments are not supported by our tokenizer right now
      case bustub_libpgquery::PGSimplifiedTokenType::PG_SIMPLIFIED_TOKEN_COMMENT:
        token.type_ = SimplifiedTokenType::SIMPLIFIED_TOKEN_COMMENT;
        break;
      default:
        throw Exception("Unrecognized token category");
    }
    token.start_ = pg_token.start;
    result.push_back(token);
  }
  return result;
}

bool Parser::TransformParseTree(bustub_libpgquery::PGList *tree, vector<unique_ptr<SQLStatement>> &statements) {
  for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
    auto stmt = TransformStatement(static_cast<bustub_libpgquery::PGNode *>(entry->data.ptr_value));
    statements.push_back(move(stmt));
  }
  return true;
}

unique_ptr<SQLStatement> Parser::TransformStatement(bustub_libpgquery::PGNode *stmt) {
  switch (stmt->type) {
    case bustub_libpgquery::T_PGRawStmt: {
      auto raw_stmt = reinterpret_cast<bustub_libpgquery::PGRawStmt *>(stmt);
      auto result = TransformStatement(raw_stmt->stmt);
      if (result) {
        result->stmt_location_ = raw_stmt->stmt_location;
        result->stmt_length_ = raw_stmt->stmt_len;
      }
      return result;
    }
    case bustub_libpgquery::T_PGCreateStmt:
      return make_unique<CreateStatement>(*this, reinterpret_cast<PGCreateStmt *>(stmt));
    case bustub_libpgquery::T_PGInsertStmt:
      return make_unique<InsertStatement>(*this, reinterpret_cast<PGInsertStmt *>(stmt));
    case bustub_libpgquery::T_PGSelectStmt:
      return std::make_unique<SelectStatement>(*this, reinterpret_cast<PGSelectStmt *>(stmt));
    case bustub_libpgquery::T_PGDeleteStmt:
      return make_unique<DeleteStatement>(*this, reinterpret_cast<PGDeleteStmt *>(stmt));
    case bustub_libpgquery::T_PGIndexStmt:
    case bustub_libpgquery::T_PGUpdateStmt:
    default:
      throw NotImplementedException(NodetypeToString(stmt->type));
  }
}

static const std::pair<std::string, TypeId> INTERNAL_TYPES[] = {
    {"int", TypeId::INTEGER},        {"int4", TypeId::INTEGER},
    {"signed", TypeId::INTEGER},     {"integer", TypeId::INTEGER},
    {"integral", TypeId::INTEGER},   {"int32", TypeId::INTEGER},
    {"varchar", TypeId::VARCHAR},    {"bpchar", TypeId::VARCHAR},
    {"text", TypeId::VARCHAR},       {"string", TypeId::VARCHAR},
    {"char", TypeId::VARCHAR},       {"nvarchar", TypeId::VARCHAR},
    {"int8", TypeId::BIGINT},        {"bigint", TypeId::BIGINT},
    {"int64", TypeId::BIGINT},       {"long", TypeId::BIGINT},
    {"oid", TypeId::BIGINT},         {"int2", TypeId::SMALLINT},
    {"smallint", TypeId::SMALLINT},  {"short", TypeId::SMALLINT},
    {"int16", TypeId::SMALLINT},     {"timestamp", TypeId::TIMESTAMP},
    {"datetime", TypeId::TIMESTAMP}, {"timestamp_us", TypeId::TIMESTAMP},
    {"bool", TypeId::BOOLEAN},       {"boolean", TypeId::BOOLEAN},
    {"logical", TypeId::BOOLEAN},    {"decimal", TypeId::DECIMAL},
    {"dec", TypeId::DECIMAL},        {"numeric", TypeId::DECIMAL},
    {"real", TypeId::DECIMAL},       {"float4", TypeId::DECIMAL},
    {"float", TypeId::DECIMAL},      {"double", TypeId::DECIMAL},
    {"float8", TypeId::DECIMAL},     {"tinyint", TypeId::TINYINT},
    {"int1", TypeId::TINYINT},       {"", TypeId::INVALID}};

TypeId Parser::TransformTypeId(bustub_libpgquery::PGTypeName *type_name) {
  if (type_name->type != bustub_libpgquery::T_PGTypeName) {
    throw Exception("Expected a type");
  }

  auto name = (reinterpret_cast<bustub_libpgquery::PGValue *>(type_name->names->tail->data.ptr_value)->val.str);
  // transform it to the SQL type
  auto lower_str = StringUtil::Lower(name);

  for (uint64_t index = 0; !INTERNAL_TYPES[index].first.empty(); index++) {
    if (INTERNAL_TYPES[index].first == lower_str) {
      return INTERNAL_TYPES[index].second;
    }
  }
  return TypeId::INVALID;
}

Column Parser::TransformColumnDefinition(bustub_libpgquery::PGColumnDef *cdef) {
  string colname;
  if (cdef->colname != nullptr) {
    colname = cdef->colname;
  }
  TypeId type_id = TransformTypeId(cdef->typeName);
  return Column(colname, type_id);
}

std::string Parser::NodetypeToString(bustub_libpgquery::PGNodeTag type) {  // LCOV_EXCL_START
  switch (type) {
    case bustub_libpgquery::T_PGInvalid:
      return "T_Invalid";
    case bustub_libpgquery::T_PGIndexInfo:
      return "T_IndexInfo";
    case bustub_libpgquery::T_PGExprContext:
      return "T_ExprContext";
    case bustub_libpgquery::T_PGProjectionInfo:
      return "T_ProjectionInfo";
    case bustub_libpgquery::T_PGJunkFilter:
      return "T_JunkFilter";
    case bustub_libpgquery::T_PGResultRelInfo:
      return "T_ResultRelInfo";
    case bustub_libpgquery::T_PGEState:
      return "T_EState";
    case bustub_libpgquery::T_PGTupleTableSlot:
      return "T_TupleTableSlot";
    case bustub_libpgquery::T_PGPlan:
      return "T_Plan";
    case bustub_libpgquery::T_PGResult:
      return "T_Result";
    case bustub_libpgquery::T_PGProjectSet:
      return "T_ProjectSet";
    case bustub_libpgquery::T_PGModifyTable:
      return "T_ModifyTable";
    case bustub_libpgquery::T_PGAppend:
      return "T_Append";
    case bustub_libpgquery::T_PGMergeAppend:
      return "T_MergeAppend";
    case bustub_libpgquery::T_PGRecursiveUnion:
      return "T_RecursiveUnion";
    case bustub_libpgquery::T_PGBitmapAnd:
      return "T_BitmapAnd";
    case bustub_libpgquery::T_PGBitmapOr:
      return "T_BitmapOr";
    case bustub_libpgquery::T_PGScan:
      return "T_Scan";
    case bustub_libpgquery::T_PGSeqScan:
      return "T_SeqScan";
    case bustub_libpgquery::T_PGSampleScan:
      return "T_SampleScan";
    case bustub_libpgquery::T_PGIndexScan:
      return "T_IndexScan";
    case bustub_libpgquery::T_PGIndexOnlyScan:
      return "T_IndexOnlyScan";
    case bustub_libpgquery::T_PGBitmapIndexScan:
      return "T_BitmapIndexScan";
    case bustub_libpgquery::T_PGBitmapHeapScan:
      return "T_BitmapHeapScan";
    case bustub_libpgquery::T_PGTidScan:
      return "T_TidScan";
    case bustub_libpgquery::T_PGSubqueryScan:
      return "T_SubqueryScan";
    case bustub_libpgquery::T_PGFunctionScan:
      return "T_FunctionScan";
    case bustub_libpgquery::T_PGValuesScan:
      return "T_ValuesScan";
    case bustub_libpgquery::T_PGTableFuncScan:
      return "T_TableFuncScan";
    case bustub_libpgquery::T_PGCteScan:
      return "T_CteScan";
    case bustub_libpgquery::T_PGNamedTuplestoreScan:
      return "T_NamedTuplestoreScan";
    case bustub_libpgquery::T_PGWorkTableScan:
      return "T_WorkTableScan";
    case bustub_libpgquery::T_PGForeignScan:
      return "T_ForeignScan";
    case bustub_libpgquery::T_PGCustomScan:
      return "T_CustomScan";
    case bustub_libpgquery::T_PGJoin:
      return "T_Join";
    case bustub_libpgquery::T_PGNestLoop:
      return "T_NestLoop";
    case bustub_libpgquery::T_PGMergeJoin:
      return "T_MergeJoin";
    case bustub_libpgquery::T_PGHashJoin:
      return "T_HashJoin";
    case bustub_libpgquery::T_PGMaterial:
      return "T_Material";
    case bustub_libpgquery::T_PGSort:
      return "T_Sort";
    case bustub_libpgquery::T_PGGroup:
      return "T_Group";
    case bustub_libpgquery::T_PGAgg:
      return "T_Agg";
    case bustub_libpgquery::T_PGWindowAgg:
      return "T_WindowAgg";
    case bustub_libpgquery::T_PGUnique:
      return "T_Unique";
    case bustub_libpgquery::T_PGGather:
      return "T_Gather";
    case bustub_libpgquery::T_PGGatherMerge:
      return "T_GatherMerge";
    case bustub_libpgquery::T_PGHash:
      return "T_Hash";
    case bustub_libpgquery::T_PGSetOp:
      return "T_SetOp";
    case bustub_libpgquery::T_PGLockRows:
      return "T_LockRows";
    case bustub_libpgquery::T_PGLimit:
      return "T_Limit";
    case bustub_libpgquery::T_PGNestLoopParam:
      return "T_NestLoopParam";
    case bustub_libpgquery::T_PGPlanRowMark:
      return "T_PlanRowMark";
    case bustub_libpgquery::T_PGPlanInvalItem:
      return "T_PlanInvalItem";
    case bustub_libpgquery::T_PGPlanState:
      return "T_PlanState";
    case bustub_libpgquery::T_PGResultState:
      return "T_ResultState";
    case bustub_libpgquery::T_PGProjectSetState:
      return "T_ProjectSetState";
    case bustub_libpgquery::T_PGModifyTableState:
      return "T_ModifyTableState";
    case bustub_libpgquery::T_PGAppendState:
      return "T_AppendState";
    case bustub_libpgquery::T_PGMergeAppendState:
      return "T_MergeAppendState";
    case bustub_libpgquery::T_PGRecursiveUnionState:
      return "T_RecursiveUnionState";
    case bustub_libpgquery::T_PGBitmapAndState:
      return "T_BitmapAndState";
    case bustub_libpgquery::T_PGBitmapOrState:
      return "T_BitmapOrState";
    case bustub_libpgquery::T_PGScanState:
      return "T_ScanState";
    case bustub_libpgquery::T_PGSeqScanState:
      return "T_SeqScanState";
    case bustub_libpgquery::T_PGSampleScanState:
      return "T_SampleScanState";
    case bustub_libpgquery::T_PGIndexScanState:
      return "T_IndexScanState";
    case bustub_libpgquery::T_PGIndexOnlyScanState:
      return "T_IndexOnlyScanState";
    case bustub_libpgquery::T_PGBitmapIndexScanState:
      return "T_BitmapIndexScanState";
    case bustub_libpgquery::T_PGBitmapHeapScanState:
      return "T_BitmapHeapScanState";
    case bustub_libpgquery::T_PGTidScanState:
      return "T_TidScanState";
    case bustub_libpgquery::T_PGSubqueryScanState:
      return "T_SubqueryScanState";
    case bustub_libpgquery::T_PGFunctionScanState:
      return "T_FunctionScanState";
    case bustub_libpgquery::T_PGTableFuncScanState:
      return "T_TableFuncScanState";
    case bustub_libpgquery::T_PGValuesScanState:
      return "T_ValuesScanState";
    case bustub_libpgquery::T_PGCteScanState:
      return "T_CteScanState";
    case bustub_libpgquery::T_PGNamedTuplestoreScanState:
      return "T_NamedTuplestoreScanState";
    case bustub_libpgquery::T_PGWorkTableScanState:
      return "T_WorkTableScanState";
    case bustub_libpgquery::T_PGForeignScanState:
      return "T_ForeignScanState";
    case bustub_libpgquery::T_PGCustomScanState:
      return "T_CustomScanState";
    case bustub_libpgquery::T_PGJoinState:
      return "T_JoinState";
    case bustub_libpgquery::T_PGNestLoopState:
      return "T_NestLoopState";
    case bustub_libpgquery::T_PGMergeJoinState:
      return "T_MergeJoinState";
    case bustub_libpgquery::T_PGHashJoinState:
      return "T_HashJoinState";
    case bustub_libpgquery::T_PGMaterialState:
      return "T_MaterialState";
    case bustub_libpgquery::T_PGSortState:
      return "T_SortState";
    case bustub_libpgquery::T_PGGroupState:
      return "T_GroupState";
    case bustub_libpgquery::T_PGAggState:
      return "T_AggState";
    case bustub_libpgquery::T_PGWindowAggState:
      return "T_WindowAggState";
    case bustub_libpgquery::T_PGUniqueState:
      return "T_UniqueState";
    case bustub_libpgquery::T_PGGatherState:
      return "T_GatherState";
    case bustub_libpgquery::T_PGGatherMergeState:
      return "T_GatherMergeState";
    case bustub_libpgquery::T_PGHashState:
      return "T_HashState";
    case bustub_libpgquery::T_PGSetOpState:
      return "T_SetOpState";
    case bustub_libpgquery::T_PGLockRowsState:
      return "T_LockRowsState";
    case bustub_libpgquery::T_PGLimitState:
      return "T_LimitState";
    case bustub_libpgquery::T_PGAlias:
      return "T_Alias";
    case bustub_libpgquery::T_PGRangeVar:
      return "T_RangeVar";
    case bustub_libpgquery::T_PGTableFunc:
      return "T_TableFunc";
    case bustub_libpgquery::T_PGExpr:
      return "T_Expr";
    case bustub_libpgquery::T_PGVar:
      return "T_Var";
    case bustub_libpgquery::T_PGConst:
      return "T_Const";
    case bustub_libpgquery::T_PGParam:
      return "T_Param";
    case bustub_libpgquery::T_PGAggref:
      return "T_Aggref";
    case bustub_libpgquery::T_PGGroupingFunc:
      return "T_GroupingFunc";
    case bustub_libpgquery::T_PGWindowFunc:
      return "T_WindowFunc";
    case bustub_libpgquery::T_PGArrayRef:
      return "T_ArrayRef";
    case bustub_libpgquery::T_PGFuncExpr:
      return "T_FuncExpr";
    case bustub_libpgquery::T_PGNamedArgExpr:
      return "T_NamedArgExpr";
    case bustub_libpgquery::T_PGOpExpr:
      return "T_OpExpr";
    case bustub_libpgquery::T_PGDistinctExpr:
      return "T_DistinctExpr";
    case bustub_libpgquery::T_PGNullIfExpr:
      return "T_NullIfExpr";
    case bustub_libpgquery::T_PGScalarArrayOpExpr:
      return "T_ScalarArrayOpExpr";
    case bustub_libpgquery::T_PGBoolExpr:
      return "T_BoolExpr";
    case bustub_libpgquery::T_PGSubLink:
      return "T_SubLink";
    case bustub_libpgquery::T_PGSubPlan:
      return "T_SubPlan";
    case bustub_libpgquery::T_PGAlternativeSubPlan:
      return "T_AlternativeSubPlan";
    case bustub_libpgquery::T_PGFieldSelect:
      return "T_FieldSelect";
    case bustub_libpgquery::T_PGFieldStore:
      return "T_FieldStore";
    case bustub_libpgquery::T_PGRelabelType:
      return "T_RelabelType";
    case bustub_libpgquery::T_PGCoerceViaIO:
      return "T_CoerceViaIO";
    case bustub_libpgquery::T_PGArrayCoerceExpr:
      return "T_ArrayCoerceExpr";
    case bustub_libpgquery::T_PGConvertRowtypeExpr:
      return "T_ConvertRowtypeExpr";
    case bustub_libpgquery::T_PGCollateExpr:
      return "T_CollateExpr";
    case bustub_libpgquery::T_PGCaseExpr:
      return "T_CaseExpr";
    case bustub_libpgquery::T_PGCaseWhen:
      return "T_CaseWhen";
    case bustub_libpgquery::T_PGCaseTestExpr:
      return "T_CaseTestExpr";
    case bustub_libpgquery::T_PGArrayExpr:
      return "T_ArrayExpr";
    case bustub_libpgquery::T_PGRowExpr:
      return "T_RowExpr";
    case bustub_libpgquery::T_PGRowCompareExpr:
      return "T_RowCompareExpr";
    case bustub_libpgquery::T_PGCoalesceExpr:
      return "T_CoalesceExpr";
    case bustub_libpgquery::T_PGMinMaxExpr:
      return "T_MinMaxExpr";
    case bustub_libpgquery::T_PGSQLValueFunction:
      return "T_SQLValueFunction";
    case bustub_libpgquery::T_PGXmlExpr:
      return "T_XmlExpr";
    case bustub_libpgquery::T_PGNullTest:
      return "T_NullTest";
    case bustub_libpgquery::T_PGBooleanTest:
      return "T_BooleanTest";
    case bustub_libpgquery::T_PGCoerceToDomain:
      return "T_CoerceToDomain";
    case bustub_libpgquery::T_PGCoerceToDomainValue:
      return "T_CoerceToDomainValue";
    case bustub_libpgquery::T_PGSetToDefault:
      return "T_SetToDefault";
    case bustub_libpgquery::T_PGCurrentOfExpr:
      return "T_CurrentOfExpr";
    case bustub_libpgquery::T_PGNextValueExpr:
      return "T_NextValueExpr";
    case bustub_libpgquery::T_PGInferenceElem:
      return "T_InferenceElem";
    case bustub_libpgquery::T_PGTargetEntry:
      return "T_TargetEntry";
    case bustub_libpgquery::T_PGRangeTblRef:
      return "T_RangeTblRef";
    case bustub_libpgquery::T_PGJoinExpr:
      return "T_JoinExpr";
    case bustub_libpgquery::T_PGFromExpr:
      return "T_FromExpr";
    case bustub_libpgquery::T_PGOnConflictExpr:
      return "T_OnConflictExpr";
    case bustub_libpgquery::T_PGIntoClause:
      return "T_IntoClause";
    case bustub_libpgquery::T_PGExprState:
      return "T_ExprState";
    case bustub_libpgquery::T_PGAggrefExprState:
      return "T_AggrefExprState";
    case bustub_libpgquery::T_PGWindowFuncExprState:
      return "T_WindowFuncExprState";
    case bustub_libpgquery::T_PGSetExprState:
      return "T_SetExprState";
    case bustub_libpgquery::T_PGSubPlanState:
      return "T_SubPlanState";
    case bustub_libpgquery::T_PGAlternativeSubPlanState:
      return "T_AlternativeSubPlanState";
    case bustub_libpgquery::T_PGDomainConstraintState:
      return "T_DomainConstraintState";
    case bustub_libpgquery::T_PGPlannerInfo:
      return "T_PlannerInfo";
    case bustub_libpgquery::T_PGPlannerGlobal:
      return "T_PlannerGlobal";
    case bustub_libpgquery::T_PGRelOptInfo:
      return "T_RelOptInfo";
    case bustub_libpgquery::T_PGIndexOptInfo:
      return "T_IndexOptInfo";
    case bustub_libpgquery::T_PGForeignKeyOptInfo:
      return "T_ForeignKeyOptInfo";
    case bustub_libpgquery::T_PGParamPathInfo:
      return "T_ParamPathInfo";
    case bustub_libpgquery::T_PGPath:
      return "T_Path";
    case bustub_libpgquery::T_PGIndexPath:
      return "T_IndexPath";
    case bustub_libpgquery::T_PGBitmapHeapPath:
      return "T_BitmapHeapPath";
    case bustub_libpgquery::T_PGBitmapAndPath:
      return "T_BitmapAndPath";
    case bustub_libpgquery::T_PGBitmapOrPath:
      return "T_BitmapOrPath";
    case bustub_libpgquery::T_PGTidPath:
      return "T_TidPath";
    case bustub_libpgquery::T_PGSubqueryScanPath:
      return "T_SubqueryScanPath";
    case bustub_libpgquery::T_PGForeignPath:
      return "T_ForeignPath";
    case bustub_libpgquery::T_PGCustomPath:
      return "T_CustomPath";
    case bustub_libpgquery::T_PGNestPath:
      return "T_NestPath";
    case bustub_libpgquery::T_PGMergePath:
      return "T_MergePath";
    case bustub_libpgquery::T_PGHashPath:
      return "T_HashPath";
    case bustub_libpgquery::T_PGAppendPath:
      return "T_AppendPath";
    case bustub_libpgquery::T_PGMergeAppendPath:
      return "T_MergeAppendPath";
    case bustub_libpgquery::T_PGResultPath:
      return "T_ResultPath";
    case bustub_libpgquery::T_PGMaterialPath:
      return "T_MaterialPath";
    case bustub_libpgquery::T_PGUniquePath:
      return "T_UniquePath";
    case bustub_libpgquery::T_PGGatherPath:
      return "T_GatherPath";
    case bustub_libpgquery::T_PGGatherMergePath:
      return "T_GatherMergePath";
    case bustub_libpgquery::T_PGProjectionPath:
      return "T_ProjectionPath";
    case bustub_libpgquery::T_PGProjectSetPath:
      return "T_ProjectSetPath";
    case bustub_libpgquery::T_PGSortPath:
      return "T_SortPath";
    case bustub_libpgquery::T_PGGroupPath:
      return "T_GroupPath";
    case bustub_libpgquery::T_PGUpperUniquePath:
      return "T_UpperUniquePath";
    case bustub_libpgquery::T_PGAggPath:
      return "T_AggPath";
    case bustub_libpgquery::T_PGGroupingSetsPath:
      return "T_GroupingSetsPath";
    case bustub_libpgquery::T_PGMinMaxAggPath:
      return "T_MinMaxAggPath";
    case bustub_libpgquery::T_PGWindowAggPath:
      return "T_WindowAggPath";
    case bustub_libpgquery::T_PGSetOpPath:
      return "T_SetOpPath";
    case bustub_libpgquery::T_PGRecursiveUnionPath:
      return "T_RecursiveUnionPath";
    case bustub_libpgquery::T_PGLockRowsPath:
      return "T_LockRowsPath";
    case bustub_libpgquery::T_PGModifyTablePath:
      return "T_ModifyTablePath";
    case bustub_libpgquery::T_PGLimitPath:
      return "T_LimitPath";
    case bustub_libpgquery::T_PGEquivalenceClass:
      return "T_EquivalenceClass";
    case bustub_libpgquery::T_PGEquivalenceMember:
      return "T_EquivalenceMember";
    case bustub_libpgquery::T_PGPathKey:
      return "T_PathKey";
    case bustub_libpgquery::T_PGPathTarget:
      return "T_PathTarget";
    case bustub_libpgquery::T_PGRestrictInfo:
      return "T_RestrictInfo";
    case bustub_libpgquery::T_PGPlaceHolderVar:
      return "T_PlaceHolderVar";
    case bustub_libpgquery::T_PGSpecialJoinInfo:
      return "T_SpecialJoinInfo";
    case bustub_libpgquery::T_PGAppendRelInfo:
      return "T_AppendRelInfo";
    case bustub_libpgquery::T_PGPartitionedChildRelInfo:
      return "T_PartitionedChildRelInfo";
    case bustub_libpgquery::T_PGPlaceHolderInfo:
      return "T_PlaceHolderInfo";
    case bustub_libpgquery::T_PGMinMaxAggInfo:
      return "T_MinMaxAggInfo";
    case bustub_libpgquery::T_PGPlannerParamItem:
      return "T_PlannerParamItem";
    case bustub_libpgquery::T_PGRollupData:
      return "T_RollupData";
    case bustub_libpgquery::T_PGGroupingSetData:
      return "T_GroupingSetData";
    case bustub_libpgquery::T_PGStatisticExtInfo:
      return "T_StatisticExtInfo";
    case bustub_libpgquery::T_PGMemoryContext:
      return "T_MemoryContext";
    case bustub_libpgquery::T_PGAllocSetContext:
      return "T_AllocSetContext";
    case bustub_libpgquery::T_PGSlabContext:
      return "T_SlabContext";
    case bustub_libpgquery::T_PGValue:
      return "T_Value";
    case bustub_libpgquery::T_PGInteger:
      return "T_Integer";
    case bustub_libpgquery::T_PGFloat:
      return "T_Float";
    case bustub_libpgquery::T_PGString:
      return "T_String";
    case bustub_libpgquery::T_PGBitString:
      return "T_BitString";
    case bustub_libpgquery::T_PGNull:
      return "T_Null";
    case bustub_libpgquery::T_PGList:
      return "T_List";
    case bustub_libpgquery::T_PGIntList:
      return "T_IntList";
    case bustub_libpgquery::T_PGOidList:
      return "T_OidList";
    case bustub_libpgquery::T_PGExtensibleNode:
      return "T_ExtensibleNode";
    case bustub_libpgquery::T_PGRawStmt:
      return "T_RawStmt";
    case bustub_libpgquery::T_PGQuery:
      return "T_Query";
    case bustub_libpgquery::T_PGPlannedStmt:
      return "T_PlannedStmt";
    case bustub_libpgquery::T_PGInsertStmt:
      return "T_InsertStmt";
    case bustub_libpgquery::T_PGDeleteStmt:
      return "T_DeleteStmt";
    case bustub_libpgquery::T_PGUpdateStmt:
      return "T_UpdateStmt";
    case bustub_libpgquery::T_PGSelectStmt:
      return "T_SelectStmt";
    case bustub_libpgquery::T_PGAlterTableStmt:
      return "T_AlterTableStmt";
    case bustub_libpgquery::T_PGAlterTableCmd:
      return "T_AlterTableCmd";
    case bustub_libpgquery::T_PGAlterDomainStmt:
      return "T_AlterDomainStmt";
    case bustub_libpgquery::T_PGSetOperationStmt:
      return "T_SetOperationStmt";
    case bustub_libpgquery::T_PGGrantStmt:
      return "T_GrantStmt";
    case bustub_libpgquery::T_PGGrantRoleStmt:
      return "T_GrantRoleStmt";
    case bustub_libpgquery::T_PGAlterDefaultPrivilegesStmt:
      return "T_AlterDefaultPrivilegesStmt";
    case bustub_libpgquery::T_PGClosePortalStmt:
      return "T_ClosePortalStmt";
    case bustub_libpgquery::T_PGClusterStmt:
      return "T_ClusterStmt";
    case bustub_libpgquery::T_PGCopyStmt:
      return "T_CopyStmt";
    case bustub_libpgquery::T_PGCreateStmt:
      return "T_CreateStmt";
    case bustub_libpgquery::T_PGDefineStmt:
      return "T_DefineStmt";
    case bustub_libpgquery::T_PGDropStmt:
      return "T_DropStmt";
    case bustub_libpgquery::T_PGTruncateStmt:
      return "T_TruncateStmt";
    case bustub_libpgquery::T_PGCommentStmt:
      return "T_CommentStmt";
    case bustub_libpgquery::T_PGFetchStmt:
      return "T_FetchStmt";
    case bustub_libpgquery::T_PGIndexStmt:
      return "T_IndexStmt";
    case bustub_libpgquery::T_PGCreateFunctionStmt:
      return "T_CreateFunctionStmt";
    case bustub_libpgquery::T_PGAlterFunctionStmt:
      return "T_AlterFunctionStmt";
    case bustub_libpgquery::T_PGDoStmt:
      return "T_DoStmt";
    case bustub_libpgquery::T_PGRenameStmt:
      return "T_RenameStmt";
    case bustub_libpgquery::T_PGRuleStmt:
      return "T_RuleStmt";
    case bustub_libpgquery::T_PGNotifyStmt:
      return "T_NotifyStmt";
    case bustub_libpgquery::T_PGListenStmt:
      return "T_ListenStmt";
    case bustub_libpgquery::T_PGUnlistenStmt:
      return "T_UnlistenStmt";
    case bustub_libpgquery::T_PGTransactionStmt:
      return "T_TransactionStmt";
    case bustub_libpgquery::T_PGViewStmt:
      return "T_ViewStmt";
    case bustub_libpgquery::T_PGLoadStmt:
      return "T_LoadStmt";
    case bustub_libpgquery::T_PGCreateDomainStmt:
      return "T_CreateDomainStmt";
    case bustub_libpgquery::T_PGCreatedbStmt:
      return "T_CreatedbStmt";
    case bustub_libpgquery::T_PGDropdbStmt:
      return "T_DropdbStmt";
    case bustub_libpgquery::T_PGVacuumStmt:
      return "T_VacuumStmt";
    case bustub_libpgquery::T_PGExplainStmt:
      return "T_ExplainStmt";
    case bustub_libpgquery::T_PGCreateTableAsStmt:
      return "T_CreateTableAsStmt";
    case bustub_libpgquery::T_PGCreateSeqStmt:
      return "T_CreateSeqStmt";
    case bustub_libpgquery::T_PGAlterSeqStmt:
      return "T_AlterSeqStmt";
    case bustub_libpgquery::T_PGVariableSetStmt:
      return "T_VariableSetStmt";
    case bustub_libpgquery::T_PGVariableShowStmt:
      return "T_VariableShowStmt";
    case bustub_libpgquery::T_PGVariableShowSelectStmt:
      return "T_VariableShowSelectStmt";
    case bustub_libpgquery::T_PGDiscardStmt:
      return "T_DiscardStmt";
    case bustub_libpgquery::T_PGCreateTrigStmt:
      return "T_CreateTrigStmt";
    case bustub_libpgquery::T_PGCreatePLangStmt:
      return "T_CreatePLangStmt";
    case bustub_libpgquery::T_PGCreateRoleStmt:
      return "T_CreateRoleStmt";
    case bustub_libpgquery::T_PGAlterRoleStmt:
      return "T_AlterRoleStmt";
    case bustub_libpgquery::T_PGDropRoleStmt:
      return "T_DropRoleStmt";
    case bustub_libpgquery::T_PGLockStmt:
      return "T_LockStmt";
    case bustub_libpgquery::T_PGConstraintsSetStmt:
      return "T_ConstraintsSetStmt";
    case bustub_libpgquery::T_PGReindexStmt:
      return "T_ReindexStmt";
    case bustub_libpgquery::T_PGCheckPointStmt:
      return "T_CheckPointStmt";
    case bustub_libpgquery::T_PGCreateSchemaStmt:
      return "T_CreateSchemaStmt";
    case bustub_libpgquery::T_PGAlterDatabaseStmt:
      return "T_AlterDatabaseStmt";
    case bustub_libpgquery::T_PGAlterDatabaseSetStmt:
      return "T_AlterDatabaseSetStmt";
    case bustub_libpgquery::T_PGAlterRoleSetStmt:
      return "T_AlterRoleSetStmt";
    case bustub_libpgquery::T_PGCreateConversionStmt:
      return "T_CreateConversionStmt";
    case bustub_libpgquery::T_PGCreateCastStmt:
      return "T_CreateCastStmt";
    case bustub_libpgquery::T_PGCreateOpClassStmt:
      return "T_CreateOpClassStmt";
    case bustub_libpgquery::T_PGCreateOpFamilyStmt:
      return "T_CreateOpFamilyStmt";
    case bustub_libpgquery::T_PGAlterOpFamilyStmt:
      return "T_AlterOpFamilyStmt";
    case bustub_libpgquery::T_PGPrepareStmt:
      return "T_PrepareStmt";
    case bustub_libpgquery::T_PGExecuteStmt:
      return "T_ExecuteStmt";
    case bustub_libpgquery::T_PGCallStmt:
      return "T_CallStmt";
    case bustub_libpgquery::T_PGDeallocateStmt:
      return "T_DeallocateStmt";
    case bustub_libpgquery::T_PGDeclareCursorStmt:
      return "T_DeclareCursorStmt";
    case bustub_libpgquery::T_PGCreateTableSpaceStmt:
      return "T_CreateTableSpaceStmt";
    case bustub_libpgquery::T_PGDropTableSpaceStmt:
      return "T_DropTableSpaceStmt";
    case bustub_libpgquery::T_PGAlterObjectDependsStmt:
      return "T_AlterObjectDependsStmt";
    case bustub_libpgquery::T_PGAlterObjectSchemaStmt:
      return "T_AlterObjectSchemaStmt";
    case bustub_libpgquery::T_PGAlterOwnerStmt:
      return "T_AlterOwnerStmt";
    case bustub_libpgquery::T_PGAlterOperatorStmt:
      return "T_AlterOperatorStmt";
    case bustub_libpgquery::T_PGDropOwnedStmt:
      return "T_DropOwnedStmt";
    case bustub_libpgquery::T_PGReassignOwnedStmt:
      return "T_ReassignOwnedStmt";
    case bustub_libpgquery::T_PGCompositeTypeStmt:
      return "T_CompositeTypeStmt";
    case bustub_libpgquery::T_PGCreateEnumStmt:
      return "T_CreateEnumStmt";
    case bustub_libpgquery::T_PGCreateRangeStmt:
      return "T_CreateRangeStmt";
    case bustub_libpgquery::T_PGAlterEnumStmt:
      return "T_AlterEnumStmt";
    case bustub_libpgquery::T_PGAlterTSDictionaryStmt:
      return "T_AlterTSDictionaryStmt";
    case bustub_libpgquery::T_PGAlterTSConfigurationStmt:
      return "T_AlterTSConfigurationStmt";
    case bustub_libpgquery::T_PGCreateFdwStmt:
      return "T_CreateFdwStmt";
    case bustub_libpgquery::T_PGAlterFdwStmt:
      return "T_AlterFdwStmt";
    case bustub_libpgquery::T_PGCreateForeignServerStmt:
      return "T_CreateForeignServerStmt";
    case bustub_libpgquery::T_PGAlterForeignServerStmt:
      return "T_AlterForeignServerStmt";
    case bustub_libpgquery::T_PGCreateUserMappingStmt:
      return "T_CreateUserMappingStmt";
    case bustub_libpgquery::T_PGAlterUserMappingStmt:
      return "T_AlterUserMappingStmt";
    case bustub_libpgquery::T_PGDropUserMappingStmt:
      return "T_DropUserMappingStmt";
    case bustub_libpgquery::T_PGAlterTableSpaceOptionsStmt:
      return "T_AlterTableSpaceOptionsStmt";
    case bustub_libpgquery::T_PGAlterTableMoveAllStmt:
      return "T_AlterTableMoveAllStmt";
    case bustub_libpgquery::T_PGSecLabelStmt:
      return "T_SecLabelStmt";
    case bustub_libpgquery::T_PGCreateForeignTableStmt:
      return "T_CreateForeignTableStmt";
    case bustub_libpgquery::T_PGImportForeignSchemaStmt:
      return "T_ImportForeignSchemaStmt";
    case bustub_libpgquery::T_PGCreateExtensionStmt:
      return "T_CreateExtensionStmt";
    case bustub_libpgquery::T_PGAlterExtensionStmt:
      return "T_AlterExtensionStmt";
    case bustub_libpgquery::T_PGAlterExtensionContentsStmt:
      return "T_AlterExtensionContentsStmt";
    case bustub_libpgquery::T_PGCreateEventTrigStmt:
      return "T_CreateEventTrigStmt";
    case bustub_libpgquery::T_PGAlterEventTrigStmt:
      return "T_AlterEventTrigStmt";
    case bustub_libpgquery::T_PGRefreshMatViewStmt:
      return "T_RefreshMatViewStmt";
    case bustub_libpgquery::T_PGReplicaIdentityStmt:
      return "T_ReplicaIdentityStmt";
    case bustub_libpgquery::T_PGAlterSystemStmt:
      return "T_AlterSystemStmt";
    case bustub_libpgquery::T_PGCreatePolicyStmt:
      return "T_CreatePolicyStmt";
    case bustub_libpgquery::T_PGAlterPolicyStmt:
      return "T_AlterPolicyStmt";
    case bustub_libpgquery::T_PGCreateTransformStmt:
      return "T_CreateTransformStmt";
    case bustub_libpgquery::T_PGCreateAmStmt:
      return "T_CreateAmStmt";
    case bustub_libpgquery::T_PGCreatePublicationStmt:
      return "T_CreatePublicationStmt";
    case bustub_libpgquery::T_PGAlterPublicationStmt:
      return "T_AlterPublicationStmt";
    case bustub_libpgquery::T_PGCreateSubscriptionStmt:
      return "T_CreateSubscriptionStmt";
    case bustub_libpgquery::T_PGAlterSubscriptionStmt:
      return "T_AlterSubscriptionStmt";
    case bustub_libpgquery::T_PGDropSubscriptionStmt:
      return "T_DropSubscriptionStmt";
    case bustub_libpgquery::T_PGCreateStatsStmt:
      return "T_CreateStatsStmt";
    case bustub_libpgquery::T_PGAlterCollationStmt:
      return "T_AlterCollationStmt";
    case bustub_libpgquery::T_PGAExpr:
      return "TAExpr";
    case bustub_libpgquery::T_PGColumnRef:
      return "T_ColumnRef";
    case bustub_libpgquery::T_PGParamRef:
      return "T_ParamRef";
    case bustub_libpgquery::T_PGAConst:
      return "TAConst";
    case bustub_libpgquery::T_PGFuncCall:
      return "T_FuncCall";
    case bustub_libpgquery::T_PGAStar:
      return "TAStar";
    case bustub_libpgquery::T_PGAIndices:
      return "TAIndices";
    case bustub_libpgquery::T_PGAIndirection:
      return "TAIndirection";
    case bustub_libpgquery::T_PGAArrayExpr:
      return "TAArrayExpr";
    case bustub_libpgquery::T_PGResTarget:
      return "T_ResTarget";
    case bustub_libpgquery::T_PGMultiAssignRef:
      return "T_MultiAssignRef";
    case bustub_libpgquery::T_PGTypeCast:
      return "T_TypeCast";
    case bustub_libpgquery::T_PGCollateClause:
      return "T_CollateClause";
    case bustub_libpgquery::T_PGSortBy:
      return "T_SortBy";
    case bustub_libpgquery::T_PGWindowDef:
      return "T_WindowDef";
    case bustub_libpgquery::T_PGRangeSubselect:
      return "T_RangeSubselect";
    case bustub_libpgquery::T_PGRangeFunction:
      return "T_RangeFunction";
    case bustub_libpgquery::T_PGRangeTableSample:
      return "T_RangeTableSample";
    case bustub_libpgquery::T_PGRangeTableFunc:
      return "T_RangeTableFunc";
    case bustub_libpgquery::T_PGRangeTableFuncCol:
      return "T_RangeTableFuncCol";
    case bustub_libpgquery::T_PGTypeName:
      return "T_TypeName";
    case bustub_libpgquery::T_PGColumnDef:
      return "T_ColumnDef";
    case bustub_libpgquery::T_PGIndexElem:
      return "T_IndexElem";
    case bustub_libpgquery::T_PGConstraint:
      return "T_Constraint";
    case bustub_libpgquery::T_PGDefElem:
      return "T_DefElem";
    case bustub_libpgquery::T_PGRangeTblEntry:
      return "T_RangeTblEntry";
    case bustub_libpgquery::T_PGRangeTblFunction:
      return "T_RangeTblFunction";
    case bustub_libpgquery::T_PGTableSampleClause:
      return "T_TableSampleClause";
    case bustub_libpgquery::T_PGWithCheckOption:
      return "T_WithCheckOption";
    case bustub_libpgquery::T_PGSortGroupClause:
      return "T_SortGroupClause";
    case bustub_libpgquery::T_PGGroupingSet:
      return "T_GroupingSet";
    case bustub_libpgquery::T_PGWindowClause:
      return "T_WindowClause";
    case bustub_libpgquery::T_PGObjectWithArgs:
      return "T_ObjectWithArgs";
    case bustub_libpgquery::T_PGAccessPriv:
      return "T_AccessPriv";
    case bustub_libpgquery::T_PGCreateOpClassItem:
      return "T_CreateOpClassItem";
    case bustub_libpgquery::T_PGTableLikeClause:
      return "T_TableLikeClause";
    case bustub_libpgquery::T_PGFunctionParameter:
      return "T_FunctionParameter";
    case bustub_libpgquery::T_PGLockingClause:
      return "T_LockingClause";
    case bustub_libpgquery::T_PGRowMarkClause:
      return "T_RowMarkClause";
    case bustub_libpgquery::T_PGXmlSerialize:
      return "T_XmlSerialize";
    case bustub_libpgquery::T_PGWithClause:
      return "T_WithClause";
    case bustub_libpgquery::T_PGInferClause:
      return "T_InferClause";
    case bustub_libpgquery::T_PGOnConflictClause:
      return "T_OnConflictClause";
    case bustub_libpgquery::T_PGCommonTableExpr:
      return "T_CommonTableExpr";
    case bustub_libpgquery::T_PGRoleSpec:
      return "T_RoleSpec";
    case bustub_libpgquery::T_PGTriggerTransition:
      return "T_TriggerTransition";
    case bustub_libpgquery::T_PGPartitionElem:
      return "T_PartitionElem";
    case bustub_libpgquery::T_PGPartitionSpec:
      return "T_PartitionSpec";
    case bustub_libpgquery::T_PGPartitionBoundSpec:
      return "T_PartitionBoundSpec";
    case bustub_libpgquery::T_PGPartitionRangeDatum:
      return "T_PartitionRangeDatum";
    case bustub_libpgquery::T_PGPartitionCmd:
      return "T_PartitionCmd";
    case bustub_libpgquery::T_PGIdentifySystemCmd:
      return "T_IdentifySystemCmd";
    case bustub_libpgquery::T_PGBaseBackupCmd:
      return "T_BaseBackupCmd";
    case bustub_libpgquery::T_PGCreateReplicationSlotCmd:
      return "T_CreateReplicationSlotCmd";
    case bustub_libpgquery::T_PGDropReplicationSlotCmd:
      return "T_DropReplicationSlotCmd";
    case bustub_libpgquery::T_PGStartReplicationCmd:
      return "T_StartReplicationCmd";
    case bustub_libpgquery::T_PGTimeLineHistoryCmd:
      return "T_TimeLineHistoryCmd";
    case bustub_libpgquery::T_PGSQLCmd:
      return "T_SQLCmd";
    case bustub_libpgquery::T_PGTriggerData:
      return "T_TriggerData";
    case bustub_libpgquery::T_PGEventTriggerData:
      return "T_EventTriggerData";
    case bustub_libpgquery::T_PGReturnSetInfo:
      return "T_ReturnSetInfo";
    case bustub_libpgquery::T_PGWindowObjectData:
      return "T_WindowObjectData";
    case bustub_libpgquery::T_PGTIDBitmap:
      return "T_TIDBitmap";
    case bustub_libpgquery::T_PGInlineCodeBlock:
      return "T_InlineCodeBlock";
    case bustub_libpgquery::T_PGFdwRoutine:
      return "T_FdwRoutine";
    case bustub_libpgquery::T_PGIndexAmRoutine:
      return "T_IndexAmRoutine";
    case bustub_libpgquery::T_PGTsmRoutine:
      return "T_TsmRoutine";
    case bustub_libpgquery::T_PGForeignKeyCacheInfo:
      return "T_ForeignKeyCacheInfo";
    default:
      return "(UNKNOWN)";
  }
}

}  // namespace bustub
