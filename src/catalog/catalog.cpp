#include "catalog/catalog.h"

namespace bustub {
class DatabaseInstance;

Catalog::Catalog(DatabaseInstance &db) : db_(db){};

/**
 * Create a new table and return its metadata.
 * @param txn The transaction in which the table is being created
 * @param table_name The name of the new table
 * @param schema The schema of the new table
 * @return A (non-owning) pointer to the metadata for the table
 */
TableInfo *Catalog::CreateTable(Transaction *txn, const std::string &table_name, const Schema &schema) {
  if (table_names_.count(table_name) != 0) {
    return NULL_TABLE_INFO;
  }

  // Construct the table heap
  auto table = std::make_unique<TableHeap>(db_, txn);

  // Fetch the table OID for the new table
  const auto table_oid = next_table_oid_.fetch_add(1);

  // Construct the table information
  auto meta = std::make_unique<TableInfo>(schema, table_name, std::move(table), table_oid);
  auto *tmp = meta.get();

  // Update the internal tracking mechanisms
  tables_.emplace(table_oid, std::move(meta));
  table_names_.emplace(table_name, table_oid);
  index_names_.emplace(table_name, std::unordered_map<std::string, index_oid_t>{});

  return tmp;
}

/**
 * Query table metadata by name.
 * @param table_name The name of the table
 * @return A (non-owning) pointer to the metadata for the table
 */
TableInfo *Catalog::GetTable(const std::string &table_name) {
  auto table_oid = table_names_.find(table_name);
  if (table_oid == table_names_.end()) {
    // Table not found
    return NULL_TABLE_INFO;
  }

  auto meta = tables_.find(table_oid->second);
  BUSTUB_ASSERT(meta != tables_.end(), "Broken Invariant");

  return (meta->second).get();
}

/**
 * Query table metadata by OID
 * @param table_oid The OID of the table to query
 * @return A (non-owning) pointer to the metadata for the table
 */
TableInfo *Catalog::GetTable(table_oid_t table_oid) {
  auto meta = tables_.find(table_oid);
  if (meta == tables_.end()) {
    return NULL_TABLE_INFO;
  }

  return (meta->second).get();
}

/**
 * Create a new index, populate existing data of the table and return its metadata.
 * @param txn The transaction in which the table is being created
 * @param index_name The name of the new index
 * @param table_name The name of the table
 * @param schema The schema of the table
 * @param key_schema The schema of the key
 * @param key_attrs Key attributes
 * @param keysize Size of the key
 * @param hash_function The hash function for the index
 * @return A (non-owning) pointer to the metadata of the new table
 */
template <class KeyType, class ValueType, class KeyComparator>
IndexInfo *Catalog::CreateIndex(Transaction *txn, const std::string &index_name, const std::string &table_name,
                                const Schema &schema, const Schema &key_schema, const std::vector<uint32_t> &key_attrs,
                                std::size_t keysize, HashFunction<KeyType> hash_function) {
  // Reject the creation request for nonexistent table
  if (table_names_.find(table_name) == table_names_.end()) {
    return NULL_INDEX_INFO;
  }

  // If the table exists, an entry for the table should already be present in index_names_
  BUSTUB_ASSERT((index_names_.find(table_name) != index_names_.end()), "Broken Invariant");

  // Determine if the requested index already exists for this table
  auto &table_indexes = index_names_.find(table_name)->second;
  if (table_indexes.find(index_name) != table_indexes.end()) {
    // The requested index already exists for this table
    return NULL_INDEX_INFO;
  }

  // Construct index metdata
  auto meta = std::make_unique<IndexMetadata>(index_name, table_name, &schema, key_attrs);

  // Construct the index, take ownership of metadata
  // TODO(Kyle): We should update the API for CreateIndex
  // to allow specification of the index type itself, not
  // just the key, value, and comparator types
  auto index = std::make_unique<ExtendibleHashTableIndex<KeyType, ValueType, KeyComparator>>(
      std::move(meta), db_.GetBufferPoolManager(), hash_function);

  // Populate the index with all tuples in table heap
  auto *table_meta = GetTable(table_name);
  auto *heap = table_meta->table_.get();
  for (auto tuple = heap->Begin(txn); tuple != heap->End(); ++tuple) {
    index->InsertEntry(tuple->KeyFromTuple(schema, key_schema, key_attrs), tuple->GetRid(), txn);
  }

  // Get the next OID for the new index
  const auto index_oid = next_index_oid_.fetch_add(1);

  // Construct index information; IndexInfo takes ownership of the Index itself
  auto index_info =
      std::make_unique<IndexInfo>(key_schema, index_name, std::move(index), index_oid, table_name, keysize);
  auto *tmp = index_info.get();

  // Update internal tracking
  indexes_.emplace(index_oid, std::move(index_info));
  table_indexes.emplace(index_name, index_oid);

  return tmp;
}

/**
 * Get the index `index_name` for table `table_name`.
 * @param index_name The name of the index for which to query
 * @param table_name The name of the table on which to perform query
 * @return A (non-owning) pointer to the metadata for the index
 */
IndexInfo *Catalog::GetIndex(const std::string &index_name, const std::string &table_name) {
  auto table = index_names_.find(table_name);
  if (table == index_names_.end()) {
    BUSTUB_ASSERT((table_names_.find(table_name) == table_names_.end()), "Broken Invariant");
    return NULL_INDEX_INFO;
  }

  auto &table_indexes = table->second;

  auto index_meta = table_indexes.find(index_name);
  if (index_meta == table_indexes.end()) {
    return NULL_INDEX_INFO;
  }

  auto index = indexes_.find(index_meta->second);
  BUSTUB_ASSERT((index != indexes_.end()), "Broken Invariant");

  return index->second.get();
}

/**
 * Get the index `index_name` for table identified by `table_oid`.
 * @param index_name The name of the index for which to query
 * @param table_oid The OID of the table on which to perform query
 * @return A (non-owning) pointer to the metadata for the index
 */
IndexInfo *Catalog::GetIndex(const std::string &index_name, const table_oid_t table_oid) {
  // Locate the table metadata for the specified table OID
  auto table_meta = tables_.find(table_oid);
  if (table_meta == tables_.end()) {
    // Table not found
    return NULL_INDEX_INFO;
  }

  return GetIndex(index_name, table_meta->second->name_);
}

/**
 * Get the index identifier by index OID.
 * @param index_oid The OID of the index for which to query
 * @return A (non-owning) pointer to the metadata for the index
 */
IndexInfo *Catalog::GetIndex(index_oid_t index_oid) {
  auto index = indexes_.find(index_oid);
  if (index == indexes_.end()) {
    return NULL_INDEX_INFO;
  }

  return index->second.get();
}

/**
 * Get all of the indexes for the table identified by `table_name`.
 * @param table_name The name of the table for which indexes should be retrieved
 * @return A vector of IndexInfo* for each index on the given table, empty vector
 * in the event that the table exists but no indexes have been created for it
 */
std::vector<IndexInfo *> Catalog::GetTableIndexes(const std::string &table_name) {
  // Ensure the table exists
  if (table_names_.find(table_name) == table_names_.end()) {
    return std::vector<IndexInfo *>{};
  }

  auto table_indexes = index_names_.find(table_name);
  BUSTUB_ASSERT((table_indexes != index_names_.end()), "Broken Invariant");

  std::vector<IndexInfo *> indexes{};
  indexes.reserve(table_indexes->second.size());
  for (const auto &index_meta : table_indexes->second) {
    auto index = indexes_.find(index_meta.second);
    BUSTUB_ASSERT((index != indexes_.end()), "Broken Invariant");
    indexes.push_back(index->second.get());
  }

  return indexes;
}

}  // namespace bustub
