//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/serializer.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace bustub {
using std::string;
using std::unique_ptr;
using std::vector;

//! data pointers
using data_t = uint8_t;
using data_ptr_t = data_t *;
using const_data_ptr_t = const data_t *;

//! The Serialize class is a base class that can be used to serializing objects into a binary buffer
class Serializer {
 public:
  virtual ~Serializer() = default;

  virtual void WriteData(const_data_ptr_t buffer, uint64_t write_size) = 0;

  template <class T>
  void Write(T element) {
    static_assert(std::is_trivially_destructible<T>(), "Write element must be trivially destructible");

    WriteData((const_data_ptr_t)&element, sizeof(T));
  }

  //! Write data from a string buffer directly (without length prefix)
  void WriteBufferData(const string &str) { WriteData(reinterpret_cast<const_data_ptr_t>(str.c_str()), str.size()); }
  //! Write a string with a length prefix
  void WriteString(const string &val) { WriteStringLen(reinterpret_cast<const_data_ptr_t>(val.c_str()), val.size()); }
  void WriteStringLen(const_data_ptr_t val, uint64_t len) {
    Write<uint32_t>(static_cast<uint32_t>(len));
    if (len > 0) {
      WriteData(val, len);
    }
  }

  template <class T>
  void WriteList(const vector<unique_ptr<T>> &list) {
    Write<uint32_t>((uint32_t)list.size());
    for (auto &child : list) {
      child->Serialize(*this);
    }
  }

  void WriteStringVector(const vector<string> &list) {
    Write<uint32_t>(static_cast<uint32_t>(list.size()));
    for (auto &child : list) {
      WriteString(child);
    }
  }

  template <class T>
  void WriteOptional(const unique_ptr<T> &element) {
    Write<bool>(static_cast<bool>(element));
    if (element) {
      element->Serialize(*this);
    }
  }
};

//! The Deserializer class assists in deserializing a binary blob back into an
//! object
class Deserializer {
 public:
  virtual ~Deserializer() = default;

  //! Reads [read_size] bytes into the buffer
  virtual void ReadData(data_ptr_t buffer, uint64_t read_size) = 0;

  template <class T>
  T Read() {
    T value;
    ReadData((data_ptr_t)&value, sizeof(T));
    return value;
  }
  template <class T>
  void ReadList(vector<unique_ptr<T>> &list) {
    auto select_count = Read<uint32_t>();
    for (uint32_t i = 0; i < select_count; i++) {
      auto child = T::Deserialize(*this);
      list.push_back(move(child));
    }
  }

  template <class T, class RETURN_TYPE = T>
  unique_ptr<RETURN_TYPE> ReadOptional() {
    auto has_entry = Read<bool>();
    if (has_entry) {
      return T::Deserialize(*this);
    }
    return nullptr;
  }

  void ReadStringVector(vector<string> &list);
};

template <>
string Deserializer::Read();

}  // namespace bustub