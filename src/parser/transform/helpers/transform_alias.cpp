#include "parser/transformer.h"

namespace bustub {

string Transformer::TransformAlias(bustub_libpgquery::PGAlias *root, vector<string> &column_name_alias) {
  if (root == nullptr) {
    return "";
  }
  if (root->colnames != nullptr) {
    for (auto node = root->colnames->head; node != nullptr; node = node->next) {
      column_name_alias.emplace_back(reinterpret_cast<bustub_libpgquery::PGValue *>(node->data.ptr_value)->val.str);
    }
  }
  return root->aliasname;
}

}  // namespace bustub