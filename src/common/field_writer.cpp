#include "common/field_writer.h"

namespace bustub {

//===--------------------------------------------------------------------===//
// Field Writer
//===--------------------------------------------------------------------===//
// FieldWriter::FieldWriter(Serializer &serializer_p)
//     : serializer_(serializer_p), buffer(std::make_unique<BufferedSerializer>()), field_count_(0), finalized_(false) {
// }
FieldWriter::FieldWriter(Serializer &serializer_p) : serializer_(serializer_p), field_count_(0), finalized_(false) {}

void FieldWriter::WriteData(const_data_ptr_t buffer_ptr, uint64_t write_size) {
  // D_ASSERT(buffer);
  // buffer->WriteData(buffer_ptr, write_size);
}

template <>
void FieldWriter::Write(const string &val) {
  Write<uint32_t>(static_cast<uint32_t>(val.size()));
  if (!val.empty()) {
    WriteData(reinterpret_cast<const_data_ptr_t>(val.c_str()), val.size());
  }
}

void FieldWriter::Finalize() {
  // D_ASSERT(buffer);
  // D_ASSERT(!finalized);
  finalized_ = true;
  serializer_.Write<uint32_t>(field_count_);
  // serializer_.Write<uint64_t>(buffer->blob.size);
  // serializer_.WriteData(buffer_->blob.data.get(), buffer->blob.size);

  // buffer.reset();
}

//===--------------------------------------------------------------------===//
// Field Deserializer
//===--------------------------------------------------------------------===//
FieldDeserializer::FieldDeserializer(Deserializer &root) : root_(root), remaining_data_(static_cast<uint64_t>(-1)) {}

void FieldDeserializer::ReadData(data_ptr_t buffer, uint64_t read_size) {
  // D_ASSERT(remaining_data != uint64_t(-1));
  // D_ASSERT(read_size <= remaining_data);
  root_.ReadData(buffer, read_size);
  remaining_data_ -= read_size;
}

uint64_t FieldDeserializer::RemainingData() { return remaining_data_; }

void FieldDeserializer::SetRemainingData(uint64_t remaining_data) { this->remaining_data_ = remaining_data; }

//===--------------------------------------------------------------------===//
// Field Reader
//===--------------------------------------------------------------------===//
FieldReader::FieldReader(Deserializer &source_p) : source_(source_p), field_count_(0), finalized_(false) {
  max_field_count_ = source_p.Read<uint32_t>();
  total_size_ = source_p.Read<uint64_t>();
  // D_ASSERT(max_field_count > 0);
  source_.SetRemainingData(total_size_);
}

void FieldReader::Finalize() {
  // D_ASSERT(!finalized);
  finalized_ = true;
  if (field_count_ < max_field_count_) {
    // we can handle this case by calling source.ReadData(buffer, source.RemainingData())
    throw Exception(
        "Not all fields were read. This file might have been written with a newer version "
        "of BusTub and is incompatible with this version of BusTub.");
  }
  // D_ASSERT(source.RemainingData() == 0);
}

}  // namespace bustub
