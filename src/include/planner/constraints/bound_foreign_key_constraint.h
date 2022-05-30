//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/constraints/bound_foreign_key_constraint.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/unordered_set.h"
#include "planner/bound_constraint.h"

namespace bustub {

class BoundForeignKeyConstraint : public BoundConstraint {
 public:
  BoundForeignKeyConstraint(ForeignKeyInfo info_p, unordered_set<uint64_t> pk_key_set_p,
                            unordered_set<uint64_t> fk_key_set_p)
      : BoundConstraint(ConstraintType::FOREIGN_KEY),
        info(move(info_p)),
        pk_key_set(move(pk_key_set_p)),
        fk_key_set(move(fk_key_set_p)) {
#ifdef DEBUG
    D_ASSERT(info.pk_keys.size() == pk_key_set.size());
    for (auto &key : info.pk_keys) {
      D_ASSERT(pk_key_set.find(key) != pk_key_set.end());
    }
    D_ASSERT(info.fk_keys.size() == fk_key_set.size());
    for (auto &key : info.fk_keys) {
      D_ASSERT(fk_key_set.find(key) != fk_key_set.end());
    }
#endif
  }

  ForeignKeyInfo info;
  //! The same keys but stored as an unordered set
  unordered_set<uint64_t> pk_key_set;
  //! The same keys but stored as an unordered set
  unordered_set<uint64_t> fk_key_set;
};

}  // namespace bustub
