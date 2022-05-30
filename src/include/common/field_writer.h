//===----------------------------------------------------------------------===//
//                         BusTub
//
// common/field_writer.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include "common/exception.h"
#include "common/serializer.h"

namespace bustub {
using std::string;
using std::unique_ptr;
using std::vector;

//! data pointers
using data_t = uint8_t;
using data_ptr_t = data_t *;
using const_data_ptr_t = const data_t *;

class BufferedSerializer;

class FieldWriter {
 public:
  explicit FieldWriter(Serializer &serializer);
  ~FieldWriter() = default;

 public:
  template <class T>
  void WriteField(const T &element) {
    static_assert(std::is_trivially_destructible<T>(), "WriteField object must be trivially destructible");

    AddField();
    // WriteData((const_data_ptr_t)&element, sizeof(T));
  }

  //! Write a string with a length prefix
  void WriteString(const string &val) { WriteStringLen(reinterpret_cast<const_data_ptr_t>(val.c_str()), val.size()); }
  void WriteStringLen(const_data_ptr_t val, uint64_t len) {
    AddField();
    Write<uint32_t>(static_cast<uint32_t>(len));
    if (len > 0) {
      WriteData(val, len);
    }
  }
  void WriteBlob(const_data_ptr_t val, uint64_t len) {
    AddField();
    if (len > 0) {
      WriteData(val, len);
    }
  }

  template <class T, class CONTAINER_TYPE = vector<T>>
  void WriteList(const CONTAINER_TYPE &elements) {
    AddField();
    Write<uint32_t>(elements.size());
    for (auto &element : elements) {
      Write<T>(element);
    }
  }

  template <class T>
  void WriteSerializable(const T &element) {
    AddField();
    // element.Serialize(*buffer);
  }

  template <class T>
  void WriteSerializableList(const vector<unique_ptr<T>> &elements) {
    AddField();
    Write<uint32_t>(elements.size());
    // for (uint64_t i = 0; i < elements.size(); i++) {
    //   elements[i]->Serialize(*buffer);
    // }
  }

  template <class T>
  void WriteRegularSerializableList(const vector<T> &elements) {
    AddField();
    Write<uint32_t>(elements.size());
    // for (uint64_t i = 0; i < elements.size(); i++) {
    //   elements[i].Serialize(*buffer);
    // }
  }

  template <class T>
  void WriteOptional(const unique_ptr<T> &element) {
    AddField();
    // Write<bool>(element ? true : false);
    // if (element) {
    //   element->Serialize(*buffer);
    // }
  }

  // Called after all fields have been written. Should always be called.
  void Finalize();

  Serializer &GetSerializer() {
    Serializer &x = serializer_;
    return x;
    // return *buffer;
  }

 private:
  void AddField() { field_count_++; }

  template <class T>
  void Write(const T &element) {
    // WriteData((const_data_ptr_t)&element, sizeof(T));
  }

  void WriteData(const_data_ptr_t buffer, uint64_t write_size);

 private:
  Serializer &serializer_;
  //   unique_ptr<BufferedSerializer> buffer;
  uint64_t field_count_;
  bool finalized_;
};

// template <>
// void FieldWriter::Write(const string &val);

class FieldDeserializer : public Deserializer {
 public:
  explicit FieldDeserializer(Deserializer &root);

 public:
  void ReadData(data_ptr_t buffer, uint64_t read_size) override;

  void SetRemainingData(uint64_t remaining_data);
  uint64_t RemainingData();

 private:
  Deserializer &root_;
  uint64_t remaining_data_;
};

class FieldReader {
 public:
  explicit FieldReader(Deserializer &source);
  ~FieldReader() = default;

 public:
  template <class T>
  T ReadRequired() {
    if (field_count_ >= max_field_count_) {
      // field is not there, throw an exception
      throw Exception("Attempting to read a required field, but field is missing");
    }
    // field is there, read the actual value
    AddField();
    return source_.Read<T>();
  }

  template <class T>
  T ReadField(T default_value) {
    if (field_count_ >= max_field_count_) {
      // field is not there, read the default value
      return default_value;
    }
    // field is there, read the actual value
    AddField();
    return source_.Read<T>();
  }

  template <class T>
  vector<T> ReadRequiredList() {
    if (field_count_ >= max_field_count_) {
      // field is not there, throw an exception
      throw Exception("Attempting to read a required field, but field is missing");
    }
    AddField();
    auto result_count = source_.Read<uint32_t>();
    vector<T> result;
    for (uint64_t i = 0; i < result_count; i++) {
      result.push_back(source_.Read<T>());
    }
    return result;
  }

  template <class T>
  unique_ptr<T> ReadOptional(unique_ptr<T> default_value) {
    if (field_count_ >= max_field_count_) {
      // field is not there, read the default value
      return default_value;
    }
    // field is there, read the actual value
    AddField();
    return source_.template ReadOptional<T>();
  }

  template <class T, class RETURN_TYPE = unique_ptr<T>>
  RETURN_TYPE ReadSerializable(RETURN_TYPE default_value) {
    if (field_count_ >= max_field_count_) {
      // field is not there, read the default value
      return default_value;
    }
    // field is there, read the actual value
    AddField();
    return T::Deserialize(source_);
  }

  template <class T, class RETURN_TYPE = unique_ptr<T>, typename... ARGS>
  RETURN_TYPE ReadSerializable(RETURN_TYPE default_value, ARGS &&...args) {
    if (field_count_ >= max_field_count_) {
      // field is not there, read the default value
      return default_value;
    }
    // field is there, read the actual value
    AddField();
    return T::Deserialize(source_, std::forward<ARGS>(args)...);
  }

  template <class T, class RETURN_TYPE = unique_ptr<T>>
  RETURN_TYPE ReadRequiredSerializable() {
    if (field_count_ >= max_field_count_) {
      // field is not there, read the default value
      throw Exception("Attempting to read mandatory field, but field is missing");
    }
    // field is there, read the actual value
    AddField();
    return T::Deserialize(source_);
  }

  // template <class T, class RETURN_TYPE = unique_ptr<T>, typename... ARGS>
  // RETURN_TYPE ReadRequiredSerializable(ARGS &&...args) {
  //   if (field_count_ >= max_field_count_) {
  //     // field is not there, read the default value
  //     throw Exception("Attempting to read mandatory field, but field is missing");
  //   }
  //   // field is there, read the actual value
  //   AddField();
  //   return T::Deserialize(source_, std::forward<ARGS>(args)...);
  // }

  // template <class T, class RETURN_TYPE = unique_ptr<T>>
  // vector<RETURN_TYPE> ReadRequiredSerializableList() {
  //   if (field_count_ >= max_field_count_) {
  //     // field is not there, read the default value
  //     throw Exception("Attempting to read mandatory field, but field is missing");
  //   }
  //   // field is there, read the actual value
  //   AddField();
  //   auto result_count = source_.Read<uint32_t>();

  //   vector<RETURN_TYPE> result;
  //   for (uint64_t i = 0; i < result_count; i++) {
  //     result.push_back(T::Deserialize(source_));
  //   }
  //   return result;
  // }

  void ReadBlob(data_ptr_t result, uint64_t read_size) {
    if (field_count_ >= max_field_count_) {
      // field is not there, throw an exception
      throw Exception("Attempting to read a required field, but field is missing");
    }
    // field is there, read the actual value
    AddField();
    source_.ReadData(result, read_size);
  }

  //! Called after all fields have been read. Should always be called.
  void Finalize();

  Deserializer &GetSource() { return source_; }

 private:
  void AddField() { field_count_++; }

 private:
  FieldDeserializer source_;
  uint64_t field_count_;
  uint64_t max_field_count_;
  uint64_t total_size_;
  bool finalized_;
};

}  // namespace bustub
