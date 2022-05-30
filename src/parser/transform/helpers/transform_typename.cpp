#include <unordered_set>
#include "common/exception.h"
#include "parser/transformer.h"
#include "type/decimal_type.h"

namespace bustub {

Type Transformer::TransformTypeName(bustub_libpgquery::PGTypeName *type_name) {
  if (type_name->type != bustub_libpgquery::T_PGTypeName) {
    throw Exception("Expected a type");
  }

  auto name = (reinterpret_cast<bustub_libpgquery::PGValue *>(type_name->names->tail->data.ptr_value)->val.str);
  // transform it to the SQL type
  TypeId base_type = TransformStringToTypeId(name);

  Type result_type;
  int8_t width, scale;
  if (base_type == TypeId::DECIMAL) {
    // default decimal width/scale
    width = 18;
    scale = 3;
  } else {
    width = 0;
    scale = 0;
  }
  // check any modifiers
  int modifier_idx = 0;
  if (type_name->typmods != nullptr) {
    for (auto node = type_name->typmods->head; node != nullptr; node = node->next) {
      auto &const_val = *(static_cast<bustub_libpgquery::PGAConst *>(node->data.ptr_value));
      if (const_val.type != bustub_libpgquery::T_PGAConst || const_val.val.type != bustub_libpgquery::T_PGInteger) {
        throw Exception("Expected an integer constant as type modifier");
      }
      if (const_val.val.val.ival < 0) {
        throw Exception("Negative modifier not supported");
      }
      if (modifier_idx == 0) {
        width = const_val.val.val.ival;
      } else if (modifier_idx == 1) {
        scale = const_val.val.val.ival;
      } else {
        throw Exception("A maximum of two modifiers is supported");
      }
      modifier_idx++;
    }
  }
  switch (base_type) {
    case TypeId::VARCHAR:
      if (modifier_idx > 1) {
        throw Exception("VARCHAR only supports a single modifier");
      }
      // FIXME: create CHECK constraint based on varchar width
      width = 0;
      result_type = Type(TypeId::VARCHAR);
      break;
    case TypeId::DECIMAL:
      if (modifier_idx == 1) {
        // only width is provided: set scale to 0
        scale = 0;
      }
      if (width <= 0 || width > Decimal::MAX_WIDTH_DECIMAL) {
        throw Exception("Width must be between 1 and %d!", (int)Decimal::MAX_WIDTH_DECIMAL);
      }
      if (scale > width) {
        throw Exception("Scale cannot be bigger than width");
      }
      result_type = Type::DECIMAL(width, scale);
      break;
    case TypeId::INTERVAL:
      if (modifier_idx > 1) {
        throw Exception("INTERVAL only supports a single modifier");
      }
      width = 0;
      result_type = Type::INTERVAL;
      break;
    default:
      if (modifier_idx > 0) {
        throw Exception("Type %s does not support any modifiers!");
      }
      result_type = Type(base_type);
      break;
  }

  return result_type;
}

}  // namespace bustub
