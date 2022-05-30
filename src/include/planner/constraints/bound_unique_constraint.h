//===----------------------------------------------------------------------===//
//                         BusTub
//
// planner/constraints/bound_unique_constraint.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_set>
#include "planner/bound_constraint.h"

namespace bustub {

class BoundUniqueConstraint : public BoundConstraint {
 public:
  BoundUniqueConstraint(vector<uint64_t> keys, unordered_set<uint64_t> key_set, bool is_primary_key)
      : BoundConstraint(ConstraintType::UNIQUE),
        keys(move(keys)),
        key_set(move(key_set)),
        is_primary_key(is_primary_key) {
#ifdef DEBUG
    assert(keys.size() == key_set.size());
    for (auto &key : keys) {
      assert(key_set.find(key) != key_set.end());
    }
#endif
  }

  //! The keys that define the unique constraint
  vector<uint64_t> keys;
  //! The same keys but stored as an unordered set
  unordered_set<uint64_t> key_set;
  //! Whether or not the unique constraint is a primary key
  bool is_primary_key;
};

}  // namespace bustub
