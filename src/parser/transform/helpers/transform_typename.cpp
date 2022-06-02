#include "common/string_util.h"
#include <unordered_set>
#include "common/exception.h"
#include "parser/transformer.h"
#include "type/decimal_type.h"

namespace bustub {

static const std::pair<std::string, TypeId> INTERNAL_TYPES[] = {
    {"int", TypeId::INTEGER},
    {"int4", TypeId::INTEGER},
    {"signed", TypeId::INTEGER},
    {"integer", TypeId::INTEGER},
    {"integral", TypeId::INTEGER},
    {"int32", TypeId::INTEGER},
    {"varchar", TypeId::VARCHAR},
    {"bpchar", TypeId::VARCHAR},
    {"text", TypeId::VARCHAR},
    {"string", TypeId::VARCHAR},
    {"char", TypeId::VARCHAR},
    {"nvarchar", TypeId::VARCHAR},
    {"int8", TypeId::BIGINT},
    {"bigint", TypeId::BIGINT},
    {"int64", TypeId::BIGINT},
    {"long", TypeId::BIGINT},
    {"oid", TypeId::BIGINT},
    {"int2", TypeId::SMALLINT},
    {"smallint", TypeId::SMALLINT},
    {"short", TypeId::SMALLINT},
    {"int16", TypeId::SMALLINT},
    {"timestamp", TypeId::TIMESTAMP},
    {"datetime", TypeId::TIMESTAMP},
    {"timestamp_us", TypeId::TIMESTAMP},
    {"bool", TypeId::BOOLEAN},
    {"boolean", TypeId::BOOLEAN},
    {"logical", TypeId::BOOLEAN},
    {"decimal", TypeId::DECIMAL},
    {"dec", TypeId::DECIMAL},
    {"numeric", TypeId::DECIMAL},
    {"real", TypeId::DECIMAL},
    {"float4", TypeId::DECIMAL},
    {"float", TypeId::DECIMAL},
    {"double", TypeId::DECIMAL},
    {"float8", TypeId::DECIMAL},
    {"tinyint", TypeId::TINYINT},
    {"int1", TypeId::TINYINT},
    //    {"null", TypeId::SQLNULL}, // TODO(GH): Probably need to handle this
    {"", TypeId::INVALID}};

Type Transformer::TransformTypeName(bustub_libpgquery::PGTypeName *type_name) {
  if (type_name->type != bustub_libpgquery::T_PGTypeName) {
    throw Exception("Expected a type");
  }

  auto name = (reinterpret_cast<bustub_libpgquery::PGValue *>(type_name->names->tail->data.ptr_value)->val.str);
  // transform it to the SQL type
  auto lower_str = StringUtil::Lower(name);

  for (uint64_t index = 0; !INTERNAL_TYPES[index].first.empty(); index++) {
    if (INTERNAL_TYPES[index].first == lower_str) {
      return Type(INTERNAL_TYPES[index].second);
    }
  }
  return Type(TypeId::INVALID);
}

}  // namespace bustub
