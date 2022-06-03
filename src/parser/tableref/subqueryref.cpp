#include "parser/tableref/subqueryref.h"

#include "common/field_writer.h"
#include "common/helper.h"
#include "common/limits.h"

namespace bustub {

string SubqueryRef::ToString() const {
  string result = "(" + subquery_->ToString() + ")";
  return BaseToString(result, column_name_alias_);
}

SubqueryRef::SubqueryRef(unique_ptr<SelectStatement> subquery_p, string alias_p)
    : TableRef(TableReferenceType::SUBQUERY), subquery_(move(subquery_p)) {
  this->alias_ = move(alias_p);
}

bool SubqueryRef::Equals(const TableRef *other_p) const {
  if (!TableRef::Equals(other_p)) {
    return false;
  }
  auto other = (SubqueryRef *)other_p;
  return subquery_->Equals(other->subquery_.get());
}

unique_ptr<TableRef> SubqueryRef::Copy() {
  auto copy = make_unique<SubqueryRef>(UniquePtrCast<SQLStatement, SelectStatement>(subquery_->Copy()), alias_);
  copy->column_name_alias_ = column_name_alias_;
  CopyProperties(*copy);
  return move(copy);
}

void SubqueryRef::Serialize(FieldWriter &writer) const {
  writer.WriteSerializable(*subquery_);
  writer.WriteList<string>(column_name_alias_);
}

unique_ptr<TableRef> SubqueryRef::Deserialize(FieldReader &reader) {
  auto subquery = reader.ReadRequiredSerializable<SelectStatement>();
  auto result = make_unique<SubqueryRef>(move(subquery));
  result->column_name_alias_ = reader.ReadRequiredList<string>();
  return move(result);
}

}  // namespace bustub
