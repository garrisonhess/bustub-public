#include "parser/tableref/emptytableref.h"
#include "planner/binder.h"
#include "planner/tableref/bound_dummytableref.h"

namespace bustub {

unique_ptr<BoundTableRef> Binder::Bind(EmptyTableRef &ref) {
  return make_unique<BoundEmptyTableRef>(GenerateTableIndex());
}

}  // namespace bustub
