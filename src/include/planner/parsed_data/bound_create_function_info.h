//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/parsed_data/bound_create_function_info.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/parsed_data/create_macro_info.h"

namespace bustub {
class CatalogEntry;

struct BoundCreateFunctionInfo {
  explicit BoundCreateFunctionInfo(unique_ptr<CreateInfo> base) : base(move(base)) {}

  //! The schema to create the table in
  SchemaCatalogEntry *schema;
  //! The base CreateInfo object
  unique_ptr<CreateInfo> base;

  CreateMacroInfo &Base() { return (CreateMacroInfo &)*base; }
};

}  // namespace bustub
