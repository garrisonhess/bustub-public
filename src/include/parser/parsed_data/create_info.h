//===----------------------------------------------------------------------===//
//                         DuckDB
//
// bustub/parser/parsed_data/create_info.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <utility>

#include "parser/parsed_data/parse_info.h"
#include "type/catalog_type.h"

namespace bustub {
using std::unique_ptr;

enum class OnCreateConflict : uint8_t {
  // Standard: throw error
  ERROR_ON_CONFLICT,
  // CREATE IF NOT EXISTS, silently do nothing on conflict
  IGNORE_ON_CONFLICT,
  // CREATE OR REPLACE
  REPLACE_ON_CONFLICT
};

struct CreateInfo : public ParseInfo {
  explicit CreateInfo(CatalogType type, string schema = "main") : type_(type), schema_(std::move(schema)) {}

  ~CreateInfo() override = default;

  //! The to-be-created catalog type
  CatalogType type_;
  //! The schema name of the entry
  string schema_;
  //! What to do on create conflict
  OnCreateConflict on_conflict_{OnCreateConflict::ERROR_ON_CONFLICT};
  //! Whether or not the entry is temporary
  bool temporary_{false};
  //! Whether or not the entry is an internal entry
  bool internal_{false};
  //! The SQL string of the CREATE statement
  string sql_;

 public:
  virtual unique_ptr<CreateInfo> Copy() const = 0;
  void CopyProperties(CreateInfo &other) const {
    other.type_ = type_;
    other.schema_ = schema_;
    other.on_conflict_ = on_conflict_;
    other.temporary_ = temporary_;
    other.internal_ = internal_;
    other.sql_ = sql_;
  }
};

}  // namespace bustub
