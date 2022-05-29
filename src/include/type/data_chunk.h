// //===----------------------------------------------------------------------===//
// //                         DuckDB
// //
// // duckdb/common/types/data_chunk.hpp
// //
// //
// //===----------------------------------------------------------------------===//

// #pragma once

// // #include "duckdb/common/common.hpp"
// // #include "duckdb/common/types/vector.hpp"
// // #include "duckdb/common/winapi.hpp"

// // #include "duckdb/common/arrow_wrapper.hpp"

// #include "common/constants.h"

// // struct ArrowArray;

// namespace duckdb {
// class VectorCache;

// //!  A Data Chunk represents a set of vectors.
// /*!
//     The data chunk class is the intermediate representation used by the
//    execution engine of DuckDB. It effectively represents a subset of a relation.
//    It holds a set of vectors that all have the same length.

//     DataChunk is initialized using the DataChunk::Initialize function by
//    providing it with a vector of TypeIds for the Vector members. By default,
//    this function will also allocate a chunk of memory in the DataChunk for the
//    vectors and all the vectors will be referencing vectors to the data owned by
//    the chunk. The reason for this behavior is that the underlying vectors can
//    become referencing vectors to other chunks as well (i.e. in the case an
//    operator does not alter the data, such as a Filter operator which only adds a
//    selection vector).

//     In addition to holding the data of the vectors, the DataChunk also owns the
//    selection vector that underlying vectors can point to.
// */
// class DataChunk {
//  public:
//   //! Creates an empty DataChunk
//   DataChunk();
//   ~DataChunk();

//   //! The vectors owned by the DataChunk.
//   vector<Vector> data;

//  public:
//   inline int64_t size() const {  // NOLINT
//     return count;
//   }
//   inline int64_t ColumnCount() const { return data.size(); }
//   inline void SetCardinality(idx_t count_p) {
//     // D_ASSERT(count_p <= capacity);
//     this->count = count_p;
//   }
//   inline void SetCardinality(const DataChunk &other) { this->count = other.size(); }
//   inline void SetCapacity(const DataChunk &other) { this->capacity = other.capacity; }

//   Value GetValue(idx_t col_idx, idx_t index) const;
//   void SetValue(idx_t col_idx, idx_t index, const Value &val);

//   //! Set the DataChunk to reference another data chunk
//   void Reference(DataChunk &chunk);
//   //! Set the DataChunk to own the data of data chunk, destroying the other chunk in the process
//   void Move(DataChunk &chunk);

//   //! Initializes the DataChunk with the specified types to an empty DataChunk
//   //! This will create one vector of the specified type for each LogicalType in the
//   //! types list. The vector will be referencing vector to the data owned by
//   //! the DataChunk.
//   void Initialize(const vector<LogicalType> &types);
//   //! Initializes an empty DataChunk with the given types. The vectors will *not* have any data allocated for them.
//   void InitializeEmpty(const vector<LogicalType> &types);
//   //! Append the other DataChunk to this one. The column count and types of
//   //! the two DataChunks have to match exactly. Throws an exception if there
//   //! is not enough space in the chunk and resize is not allowed.
//   void Append(const DataChunk &other, bool resize = false, SelectionVector *sel = nullptr, idx_t count = 0);

//   //! Destroy all data and columns owned by this DataChunk
//   void Destroy();

//   //! Copies the data from this vector to another vector.
//   void Copy(DataChunk &other, idx_t offset = 0) const;
//   void Copy(DataChunk &other, const SelectionVector &sel, const idx_t source_count, const idx_t offset = 0) const;

//   //! Splits the DataChunk in two
//   void Split(DataChunk &other, idx_t split_idx);

//   //! Fuses a DataChunk onto the right of this one, and destroys the other. Inverse of Split.
//   void Fuse(DataChunk &other);

//   //! Turn all the vectors from the chunk into flat vectors
//   void Normalify();

//   unique_ptr<VectorData[]> Orrify();

//   void Slice(const SelectionVector &sel_vector, idx_t count);
//   void Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset = 0);

//   //! Resets the DataChunk to its state right after the DataChunk::Initialize
//   //! function was called. This sets the count to 0, and resets each member
//   //! Vector to point back to the data owned by this DataChunk.
//   void Reset();

//   //! Serializes a DataChunk to a stand-alone binary blob
//   void Serialize(Serializer &serializer);
//   //! Deserializes a blob back into a DataChunk
//   void Deserialize(Deserializer &source);

//   //! Hashes the DataChunk to the target vector
//   void Hash(Vector &result);

//   //! Returns a list of types of the vectors of this data chunk
//   vector<LogicalType> GetTypes();

//   //! Converts this DataChunk to a printable string representation
//   string ToString() const;
//   void Print();

//   DataChunk(const DataChunk &) = delete;

//   //   //! Verify that the DataChunk is in a consistent, not corrupt state. DEBUG
//   //   //! FUNCTION ONLY!
//   //   DUCKDB_API void Verify();

//   //   //! export data chunk as a arrow struct array that can be imported as arrow record batch
//   //   DUCKDB_API void ToArrowArray(ArrowArray *out_array);

//  private:
//   //! The amount of tuples stored in the data chunk
//   int64_t count;
//   //! The amount of tuples that can be stored in the data chunk
//   int64_t capacity;
//   //! Vector caches, used to store data when ::Initialize is called
//   vector<VectorCache> vector_caches;
// };
// }  // namespace duckdb
