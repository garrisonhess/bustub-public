// //===----------------------------------------------------------------------===//
// //                         BusTub
// //
// // bustub/parser/tableref/crossproductref.h
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// #include "parser/tableref.h"

// namespace bustub {
// //! Represents a cross product
// class CrossProductRef : public TableRef {
//  public:
//   CrossProductRef() : TableRef(TableReferenceType::CROSS_PRODUCT) {}

//   //! The left hand side of the cross product
//   unique_ptr<TableRef> left;
//   //! The right hand side of the cross product
//   unique_ptr<TableRef> right;

//  public:
//   string ToString() const override;
//   bool Equals(const TableRef *other_p) const override;

//   unique_ptr<TableRef> Copy() override;

//   //! Serializes a blob into a CrossProductRef
//   void Serialize(FieldWriter &serializer) const override;
//   //! Deserializes a blob back into a CrossProductRef
//   static unique_ptr<TableRef> Deserialize(FieldReader &source);
// };
// }  // namespace bustub
