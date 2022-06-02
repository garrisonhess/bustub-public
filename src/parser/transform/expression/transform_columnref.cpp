#include "common/exception.h"
#include "parser/expression/columnref_expression.h"
#include "parser/expression/star_expression.h"
#include "parser/transformer.h"

namespace bustub {

unique_ptr<ParsedExpression> Transformer::TransformStarExpression(bustub_libpgquery::PGNode *node) {
  auto star = reinterpret_cast<bustub_libpgquery::PGAStar *>(node);
  auto result = std::make_unique<StarExpression>(star->relation != nullptr ? star->relation : string());
  return move(result);
}

unique_ptr<ParsedExpression> Transformer::TransformColumnRef(bustub_libpgquery::PGColumnRef *root) {
  auto fields = root->fields;
  auto head_node = static_cast<bustub_libpgquery::PGNode *>(fields->head->data.ptr_value);
  switch (head_node->type) {
    case bustub_libpgquery::T_PGString: {
      if (fields->length < 1) {
        throw Exception("Unexpected field length");
      }
      vector<string> column_names;
      for (auto node = fields->head; node != nullptr; node = node->next) {
        column_names.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue *>(node->data.ptr_value)->val.str);
      }
      auto colref = make_unique<ColumnRefExpression>(move(column_names));
      colref->query_location_ = root->location;
      return move(colref);
    }
    case bustub_libpgquery::T_PGAStar: {
      return TransformStarExpression(head_node);
    }
    default:
      throw NotImplementedException("ColumnRef not implemented!");
  }
}

}  // namespace bustub
