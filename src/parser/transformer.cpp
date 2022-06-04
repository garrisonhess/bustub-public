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
      return make_unique<CreateStatement>(*this, reinterpret_cast<bustub_libpgquery::PGCreateStmt *>(stmt));
    case bustub_libpgquery::T_PGInsertStmt:
      return make_unique<InsertStatement>(*this, reinterpret_cast<bustub_libpgquery::PGInsertStmt *>(stmt));
    case bustub_libpgquery::T_PGSelectStmt:
      return std::make_unique<SelectStatement>(*this, reinterpret_cast<bustub_libpgquery::PGSelectStmt *>(stmt));
    case bustub_libpgquery::T_PGDeleteStmt:
      return make_unique<DeleteStatement>(*this, reinterpret_cast<bustub_libpgquery::PGDeleteStmt *>(stmt));
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

Value Parser::TransformConstant(bustub_libpgquery::PGAConst *c) {
  bustub_libpgquery::PGValue pg_val = c->val;

  switch (pg_val.type) {
    case bustub_libpgquery::T_PGInteger:
      assert(pg_val.val.ival <= BUSTUB_INT32_MAX);
      return Value(TypeId::INTEGER, static_cast<int32_t>(pg_val.val.ival));
    default:
      throw NotImplementedException(
          StringUtil::Format("Constant type %s is not implemented!", NodetypeToString(pg_val.type).c_str()));
  }
}

void Parser::TransformExpressionList(bustub_libpgquery::PGList &list, vector<Value> &result) {
  for (auto node = list.head; node != nullptr; node = node->next) {
    auto target = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
    assert(target);

    switch (target->type) {
      case bustub_libpgquery::T_PGAConst: {
        auto val = TransformConstant(reinterpret_cast<bustub_libpgquery::PGAConst *>(node));
        result.push_back(val);
        break;
      }
      case bustub_libpgquery::T_PGRowExpr:
      default:
        throw NotImplementedException(
            StringUtil::Format("Expression type %s is not implemented!", NodetypeToString(target->type).c_str()));
        break;
    }
  }
}

}  // namespace bustub
