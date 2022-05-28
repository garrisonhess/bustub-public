#include "sqlite3.h"

#include "bustub.h"
#include "parser/parser.h"

#include <cassert>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <memory>
#include <string>

static char *Sqlite3Strdup(const char *str);

struct Sqlite3StringBuffer {
  //! String data
  std::unique_ptr<char[]> data_;
};

struct sqlite3_stmt {
  //! The DB object that this statement belongs to
  sqlite3 *db_;
  //! The query string
  bustub::string query_string_;
  //! The prepared statement object, if successfully prepared
  std::unique_ptr<bustub::PreparedStatement> prepared_;
  //! The result object, if successfully executed
  std::unique_ptr<bustub::QueryResult> result_;
  //! The current chunk that we are iterating over
  std::unique_ptr<bustub::Tuple> current_chunk_;
  //! The current row into the current chunk that we are iterating over
  int64_t current_row_;
  //! Bound values, used for binding to the prepared statement
  std::vector<bustub::Value> bound_values_;
  //! Names of the prepared parameters
  std::vector<bustub::string> bound_names_;
  //! The current column values converted to string, used and filled by sqlite3_column_text
  std::unique_ptr<Sqlite3StringBuffer[]> current_text_;
};

struct sqlite3 {
  std::unique_ptr<bustub::BusTub> db_;
  std::unique_ptr<bustub::Connection> con_;
  bustub::string last_error_;
};

void sqlite3_randomness(int N, void *pBuf) {
  static bool init = false;
  if (!init) {
    srand(time(nullptr));
    init = true;
  }
  unsigned char *z_buf = static_cast<unsigned char *>(pBuf);
  while ((N--) != 0) {
    unsigned char next_byte = rand() % 255;
    z_buf[N] = next_byte;
  }
}

int sqlite3_open(const char *filename, /* Database filename (UTF-8) */
                 sqlite3 **ppDb        /* OUT: SQLite db handle */
) {
  return sqlite3_open_v2(filename, ppDb, 0, nullptr);
}

int sqlite3_open_v2(const char *filename, /* Database filename (UTF-8) */
                    sqlite3 **ppDb,       /* OUT: SQLite db handle */
                    int flags,            /* Flags */
                    const char *zVfs      /* Name of VFS module to use */
) {
  if ((filename != nullptr) && strcmp(filename, ":memory:") == 0) {
    filename = nullptr;
  }
  *ppDb = nullptr;
  if (zVfs != nullptr) { /* unsupported so if set we complain */
    return SQLITE_ERROR;
  }
  sqlite3 *p_db = nullptr;
  try {
    p_db = new sqlite3();
    bustub::DBConfig config;
    // config.access_mode = AccessMode::AUTOMATIC;
    // if (flags & SQLITE_OPEN_READONLY) {
    // 	config.access_mode = AccessMode::READ_ONLY;
    // }
    p_db->db_ = std::make_unique<bustub::BusTub>(filename, &config);
    // pDb->con = make_unique<Connection>(*pDb->db);

    // ExtensionHelper::LoadAllExtensions(*pDb->db);
  } catch (std::exception &ex) {
    if (p_db != nullptr) {
      p_db->last_error_ = ex.what();
    }
    return SQLITE_ERROR;
  }
  *ppDb = p_db;
  return SQLITE_OK;
}

int sqlite3_close(sqlite3 *db) {
  delete db;
  return SQLITE_OK;
}

int sqlite3_shutdown(void) { return SQLITE_OK; }

/* In SQLite this function compiles the query into VDBE bytecode,
 * in the implementation it currently executes the query */
// TODO(xx): prepare the statement instead of executing right away
int sqlite3_prepare_v2(sqlite3 *db,           /* Database handle */
                       const char *zSql,      /* SQL statement, UTF-8 encoded */
                       int nByte,             /* Maximum length of zSql in bytes. */
                       sqlite3_stmt **ppStmt, /* OUT: Statement handle */
                       const char **pzTail    /* OUT: Pointer to unused portion of zSql */
) {
  return SQLITE_ERROR;

  // if (!db || !ppStmt || !zSql) {
  //   return SQLITE_MISUSE;
  // }
  // *ppStmt = nullptr;
  // string query = nByte < 0 ? zSql : string(zSql, nByte);
  // if (pzTail) {
  //   *pzTail = zSql + query.size();
  // }
  // try {
  //   Parser parser;
  //   parser.ParseQuery(query);
  //   if (parser.statements_.size() == 0) {
  //     return SQLITE_OK;
  //   }
  //   // extract the remainder
  //   int64_t next_location = parser.statements_[0]->stmt_location_ + parser.statements_[0]->stmt_length_;
  //   bool set_remainder = static_cast<size_t>(next_location) < query.size();

  //   // extract the first statement
  //   vector<unique_ptr<SQLStatement>> statements;
  //   statements.push_back(move(parser.statements_[0]));

  //   // PragmaHandler handler(*db->con->context);
  //   // handler.HandlePragmaStatements(statements);

  //   // if there are multiple statements here, we are dealing with an import database statement
  //   // we directly execute all statements besides the final one
  //   for (uint64_t i = 0; i + 1 < statements.size(); i++) {
  //     auto res = db->con->Query(move(statements[i]));
  //     if (!res->success) {
  //       db->last_error = res->error;
  //       return SQLITE_ERROR;
  //     }
  //   }

  //   // now prepare the query
  //   auto prepared = db->con->Prepare(move(statements.back()));
  //   if (!prepared->success_) {
  //     // failed to prepare: set the error message
  //     db->last_error = prepared->error_;
  //     return SQLITE_ERROR;
  //   }

  //   // create the statement entry
  //   unique_ptr<sqlite3_stmt> stmt = make_unique<sqlite3_stmt>();
  //   stmt->db = db;
  //   stmt->query_string = query;
  //   stmt->prepared = move(prepared);
  //   stmt->current_row = -1;
  //   for (int64_t i = 0; i < stmt->prepared->n_param_; i++) {
  //     stmt->bound_names.push_back("$" + to_string(i + 1));
  //     stmt->bound_values.push_back(Value());
  //   }

  //   // extract the remainder of the query and assign it to the pzTail
  //   if (pzTail && set_remainder) {
  //     *pzTail = zSql + next_location + 1;
  //   }

  //   *ppStmt = stmt.release();
  //   return SQLITE_OK;
  // } catch (std::exception &ex) {
  //   db->last_error = ex.what();
  //   return SQLITE_ERROR;
  // }
}

bool Sqlite3DisplayResult(bustub::StatementType type) {
  switch (type) {
    case bustub::StatementType::EXECUTE_STATEMENT:
    case bustub::StatementType::EXPLAIN_STATEMENT:
    case bustub::StatementType::PRAGMA_STATEMENT:
    case bustub::StatementType::SELECT_STATEMENT:
      return true;
    default:
      return false;
  }
}

/* Prepare the next result to be retrieved */
int sqlite3_step(sqlite3_stmt *pStmt) {
  return SQLITE_ERROR;

  // if (!pStmt) {
  //   return SQLITE_MISUSE;
  // }
  // if (!pStmt->prepared) {
  //   pStmt->db->last_error = "Attempting sqlite3_step() on a non-successfully prepared statement";
  //   return SQLITE_ERROR;
  // }
  // pStmt->current_text = nullptr;
  // if (!pStmt->result) {
  //   // no result yet! call Execute()
  //   pStmt->result = pStmt->prepared->Execute(pStmt->bound_values);
  //   // if (!pStmt->result->success_) {
  //   //   // error in execute: clear prepared statement
  //   //   pStmt->db->last_error = pStmt->result->error_;
  //   //   pStmt->prepared = nullptr;
  //   //   return SQLITE_ERROR;
  //   // }
  //   // fetch a chunk
  //   pStmt->current_chunk = pStmt->result->Fetch();
  //   pStmt->current_row = -1;
  //   // if (!sqlite3_display_result(pStmt->prepared->type)) {
  //   //   // only SELECT statements return results
  //   //   sqlite3_reset(pStmt);
  //   // }
  // }
  // if (!pStmt->current_chunk) {
  //   return SQLITE_DONE;
  // }
  // pStmt->current_row++;
  // if (pStmt->current_row >= (int32_t)pStmt->current_chunk->size()) {
  //   // have to fetch again!
  //   pStmt->current_row = 0;
  //   pStmt->current_chunk = pStmt->result->Fetch();
  //   if (!pStmt->current_chunk || pStmt->current_chunk->size() == 0) {
  //     sqlite3_reset(pStmt);
  //     return SQLITE_DONE;
  //   }
  // }
  // return SQLITE_ROW;
}

/* Execute multiple semicolon separated SQL statements
 * and execute the passed callback for each produced result,
 * largely copied from the original sqlite3 source */
int sqlite3_exec(sqlite3 *db,                /* The database on which the SQL executes */
                 const char *zSql,           /* The SQL to be executed */
                 sqlite3_callback xCallback, /* Invoke this callback routine */
                 void *pArg,                 /* First argument to xCallback() */
                 char **pzErrMsg             /* Write error messages here */
) {
  int rc = SQLITE_OK;             /* Return code */
  const char *z_leftover;         /* Tail of unprocessed SQL */
  sqlite3_stmt *p_stmt = nullptr; /* The current SQL statement */
  char **az_cols = nullptr;       /* Names of result columns */
  char **az_vals = nullptr;       /* Result values */

  if (zSql == nullptr) {
    zSql = "";
  }

  while (rc == SQLITE_OK && (zSql[0] != 0)) {
    int n_col;

    p_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, zSql, -1, &p_stmt, &z_leftover);
    if (rc != SQLITE_OK) {
      if (pzErrMsg != nullptr) {
        auto errmsg = sqlite3_errmsg(db);
        *pzErrMsg = errmsg != nullptr ? Sqlite3Strdup(errmsg) : nullptr;
      }
      continue;
    }
    if (p_stmt == nullptr) {
      /* this happens for a comment or white-space */
      zSql = z_leftover;
      continue;
    }

    n_col = sqlite3_column_count(p_stmt);
    az_cols = static_cast<char **>(malloc(n_col * sizeof(const char *)));
    az_vals = static_cast<char **>(malloc(n_col * sizeof(const char *)));
    if ((az_cols == nullptr) || (az_vals == nullptr)) {
      goto exec_out;
    }
    for (int i = 0; i < n_col; i++) {
      az_cols[i] = const_cast<char *>(sqlite3_column_name(p_stmt, i));
    }

    while (true) {
      rc = sqlite3_step(p_stmt);

      /* Invoke the callback function if required */
      if ((xCallback != nullptr) && rc == SQLITE_ROW) {
        for (int i = 0; i < n_col; i++) {
          az_vals[i] = (char *)sqlite3_column_text(p_stmt, i);
          if ((az_vals[i] == nullptr) && sqlite3_column_type(p_stmt, i) != SQLITE_NULL) {
            fprintf(stderr, "sqlite3_exec: out of memory.\n");
            goto exec_out;
          }
        }
        if (xCallback(pArg, n_col, az_vals, az_cols) != 0) {
          /* EVIDENCE-OF: R-38229-40159 If the callback function to
          ** sqlite3_exec() returns non-zero, then sqlite3_exec() will
          ** return SQLITE_ABORT. */
          rc = SQLITE_ABORT;
          sqlite3_finalize(p_stmt);
          p_stmt = nullptr;
          fprintf(stderr,
                  "sqlite3_exec: callback returned non-zero. "
                  "Aborting.\n");
          goto exec_out;
        }
      }
      if (rc == SQLITE_DONE) {
        rc = sqlite3_finalize(p_stmt);
        p_stmt = nullptr;
        zSql = z_leftover;
        while (isspace(zSql[0]) != 0) {
          zSql++;
        }
        break;
      }
      if (rc != SQLITE_ROW) {
        // error
        if (pzErrMsg != nullptr) {
          auto errmsg = sqlite3_errmsg(db);
          *pzErrMsg = errmsg != nullptr ? Sqlite3Strdup(errmsg) : nullptr;
        }
        goto exec_out;
      }
    }

    sqlite3_free(az_cols);
    sqlite3_free(az_vals);
    az_cols = nullptr;
    az_vals = nullptr;
  }

exec_out:
  if (p_stmt != nullptr) {
    sqlite3_finalize(p_stmt);
  }
  sqlite3_free(az_cols);
  sqlite3_free(az_vals);
  if (rc != SQLITE_OK && (pzErrMsg != nullptr) && (*pzErrMsg == nullptr)) {
    // error but no error message set
    *pzErrMsg = Sqlite3Strdup("Unknown error in DuckDB!");
  }
  return rc;
}

/* Return the text of the SQL that was used to prepare the statement */
const char *sqlite3_sql(sqlite3_stmt *pStmt) { return pStmt->query_string_.c_str(); }

int sqlite3_column_count(sqlite3_stmt *pStmt) {
  // if (!pStmt) {
  //   return 0;
  // }
  // return (int)pStmt->prepared->types.size();
  return -1;
}

////////////////////////////
//     sqlite3_column     //
////////////////////////////
int sqlite3_column_type(sqlite3_stmt *pStmt, int iCol) {
  return -1;

  // if (!pStmt || !pStmt->result || !pStmt->current_chunk) {
  //   return 0;
  // }
  // // if (FlatVector::IsNull(pStmt->current_chunk->data[iCol], pStmt->current_row)) {
  // //   return SQLITE_NULL;
  // // }

  // return SQLITE_INTEGER;

  // auto column_type = pStmt->result->types[iCol];
  // switch (column_type.id()) {
  // case LogicalTypeId::BOOLEAN:
  // case LogicalTypeId::TINYINT:
  // case LogicalTypeId::SMALLINT:
  // case LogicalTypeId::INTEGER:
  // case LogicalTypeId::BIGINT: /* TODO: Maybe blob? */
  // 	return SQLITE_INTEGER;
  // case LogicalTypeId::FLOAT:
  // case LogicalTypeId::DOUBLE:
  // case LogicalTypeId::DECIMAL:
  // 	return SQLITE_FLOAT;
  // case LogicalTypeId::DATE:
  // case LogicalTypeId::TIME:
  // case LogicalTypeId::TIMESTAMP:
  // case LogicalTypeId::VARCHAR:
  // case LogicalTypeId::LIST:
  // case LogicalTypeId::STRUCT:
  // 	return SQLITE_TEXT;
  // case LogicalTypeId::BLOB:
  // 	return SQLITE_BLOB;
  // default:
  // 	return 0;
  // }
  // return 0;
}

const char *sqlite3_column_name(sqlite3_stmt *pStmt, int N) {
  if (pStmt == nullptr) {
    return nullptr;
  }
  return nullptr;
  // return pStmt->prepared->names[N].c_str();
}

// static bool sqlite3_column_has_value(sqlite3_stmt *pStmt, int iCol, Type target_type, Value &val) {
//   // if (!pStmt || !pStmt->result || !pStmt->current_chunk) {
//   //   return false;
//   // }
//   // if (iCol < 0 || iCol >= (int)pStmt->result->types.size()) {
//   //   return false;
//   // }
//   // if (FlatVector::IsNull(pStmt->current_chunk->data[iCol], pStmt->current_row)) {
//   //   return false;
//   // }
//   // try {
//   //   val = pStmt->current_chunk->data[iCol].GetValue(pStmt->current_row).CastAs(target_type);
//   // } catch (...) {
//   //   return false;
//   // }
//   return true;
// }

double sqlite3_column_double(sqlite3_stmt *stmt, int iCol) {
  bustub::Value val;
  // if (!sqlite3_column_has_value(stmt, iCol, LogicalType::DOUBLE, val)) {
  //   return 0;
  // }
  // return val.value_.double_;
  return -1;
}

int sqlite3_column_int(sqlite3_stmt *stmt, int iCol) {
  // Value val;
  // if (!sqlite3_column_has_value(stmt, iCol, LogicalType::INTEGER, val)) {
  //   return 0;
  // }
  // return val.value_.integer;
  return -1;
}

sqlite3_int64 sqlite3_column_int64(sqlite3_stmt *stmt, int iCol) {
  // Value val;
  // if (!sqlite3_column_has_value(stmt, iCol, LogicalType::BIGINT, val)) {
  //   return 0;
  // }
  // return val.value_.bigint;
  return -1;
}

const unsigned char *sqlite3_column_text(sqlite3_stmt *pStmt, int iCol) {
  // Value val;
  // if (!sqlite3_column_has_value(pStmt, iCol, LogicalType::VARCHAR, val)) {
  //   return nullptr;
  // }
  // try {
  //   if (!pStmt->current_text) {
  //     pStmt->current_text = unique_ptr<sqlite3_string_buffer[]>(new
  //     sqlite3_string_buffer[pStmt->result->types.size()]);
  //   }
  //   auto &entry = pStmt->current_text[iCol];
  //   if (!entry.data) {
  //     // not initialized yet, convert the value and initialize it
  //     entry.data = unique_ptr<char[]>(new char[val.str_value.size() + 1]);
  //     memcpy(entry.data.get(), val.str_value.c_str(), val.str_value.size() + 1);
  //   }
  //   return (const unsigned char *)entry.data.get();
  // } catch (...) {
  //   // memory error!
  //   return nullptr;
  // }

  return nullptr;
}

////////////////////////////
//      sqlite3_bind      //
////////////////////////////
int sqlite3_bind_parameter_count(sqlite3_stmt *stmt) {
  // if (!stmt) {
  //   return 0;
  // }
  // return stmt->prepared->n_param;
  return -1;
}

const char *sqlite3_bind_parameter_name(sqlite3_stmt *stmt, int idx) {
  // if (!stmt) {
  //   return nullptr;
  // }
  // if (idx < 1 || idx > (int)stmt->prepared->n_param) {
  //   return nullptr;
  // }
  // return stmt->bound_names[idx - 1].c_str();
  return nullptr;
}

int sqlite3_bind_parameter_index(sqlite3_stmt *stmt, const char *zName) {
  // if (!stmt || !zName) {
  //   return 0;
  // }
  // for (int64_t i = 0; i < stmt->bound_names.size(); i++) {
  //   if (stmt->bound_names[i] == string(zName)) {
  //     return i + 1;
  //   }
  // }
  return -1;
}

int Sqlite3InternalBindValue(sqlite3_stmt *stmt, int idx, bustub::Value value) {
  // if (!stmt || !stmt->prepared || stmt->result) {
  //   return SQLITE_MISUSE;
  // }
  // if (idx < 1 || idx > (int)stmt->prepared->n_param) {
  //   return SQLITE_RANGE;
  // }
  // stmt->bound_values[idx - 1] = value;
  return SQLITE_OK;
}

int sqlite3_bind_int(sqlite3_stmt *stmt, int idx, int val) {
  // return sqlite3_internal_bind_value(stmt, idx, Value::INTEGER(val));
  return -1;
}

int sqlite3_bind_int64(sqlite3_stmt *stmt, int idx, sqlite3_int64 val) {
  // return sqlite3_internal_bind_value(stmt, idx, Value::BIGINT(val));
  return -1;
}

int sqlite3_bind_double(sqlite3_stmt *stmt, int idx, double val) {
  // return sqlite3_internal_bind_value(stmt, idx, Value::DOUBLE(val));
  return -1;
}

int sqlite3_bind_null(sqlite3_stmt *stmt, int idx) { return Sqlite3InternalBindValue(stmt, idx, bustub::Value()); }

SQLITE_API int sqlite3_bind_value(sqlite3_stmt * /*unused*/, int /*unused*/, const sqlite3_value * /*unused*/) {
  fprintf(stderr, "sqlite3_bind_value: unsupported.\n");
  return SQLITE_ERROR;
}

int sqlite3_bind_text(sqlite3_stmt *stmt, int idx, const char *val, int length, void (*free_func)(void *)) {
  // if (!val) {
  //   return SQLITE_MISUSE;
  // }
  // string value;
  // if (length < 0) {
  //   value = string(val);
  // } else {
  //   value = string(val, val + length);
  // }
  // if (free_func && ((ptrdiff_t)free_func) != -1) {
  //   free_func((void *)val);
  // }
  // try {
  //   return sqlite3_internal_bind_value(stmt, idx, Value(value));
  // } catch (std::exception &ex) {
  //   return SQLITE_ERROR;
  // }
  return 0;
}

int sqlite3_clear_bindings(sqlite3_stmt *stmt) {
  if (stmt == nullptr) {
    return SQLITE_MISUSE;
  }
  return SQLITE_OK;
}

int sqlite3_initialize(void) { return SQLITE_OK; }

int sqlite3_finalize(sqlite3_stmt *pStmt) {
  // if (pStmt) {
  //   if (pStmt->result && !pStmt->result->success) {
  //     pStmt->db->last_error = string(pStmt->result->error);
  //     delete pStmt;
  //     return SQLITE_ERROR;
  //   }

  //   delete pStmt;
  // }
  return SQLITE_OK;
}

/*
** Some systems have stricmp().  Others have strcasecmp().  Because
** there is no consistency, we will define our own.
**
** IMPLEMENTATION-OF: R-30243-02494 The sqlite3_stricmp() and
** sqlite3_strnicmp() APIs allow applications and extensions to compare
** the contents of two buffers containing UTF-8 strings in a
** case-independent fashion, using the same definition of "case
** independence" that SQLite uses internally when comparing identifiers.
*/

const unsigned char SQLITE3_UPPER_TO_LOWER[] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  97,
    98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
    154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

int Sqlite3StrICmp(const char *zLeft, const char *zRight) {
  unsigned char *a;
  unsigned char *b;
  int c;
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  for (;;) {
    c = static_cast<int>(SQLITE3_UPPER_TO_LOWER[*a]) - static_cast<int>(SQLITE3_UPPER_TO_LOWER[*b]);
    if ((c != 0) || *a == 0) {
      break;
    }
    a++;
    b++;
  }
  return c;
}

SQLITE_API int sqlite3_stricmp(const char *zLeft, const char *zRight) {
  if (zLeft == nullptr) {
    return zRight != nullptr ? -1 : 0;
  }
  if (zRight == nullptr) {
    return 1;
  }
  return Sqlite3StrICmp(zLeft, zRight);
}

SQLITE_API int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N) {
  unsigned char *a;
  unsigned char *b;
  if (zLeft == nullptr) {
    return zRight != nullptr ? -1 : 0;
  }
  if (zRight == nullptr) {
    return 1;
  }
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while (N-- > 0 && *a != 0 && SQLITE3_UPPER_TO_LOWER[*a] == SQLITE3_UPPER_TO_LOWER[*b]) {
    a++;
    b++;
  }
  return N < 0 ? 0 : SQLITE3_UPPER_TO_LOWER[*a] - SQLITE3_UPPER_TO_LOWER[*b];
}

char *Sqlite3Strdup(const char *str) {
  char *result = static_cast<char *>(sqlite3_malloc64(strlen(str) + 1));
  strcpy(result, str);
  return result;
}

void *sqlite3_malloc64(sqlite3_uint64 n) { return malloc(n); }
void sqlite3_free(void *pVoid) { free(pVoid); }

void *sqlite3_malloc(int n) { return sqlite3_malloc64(n); }

void *sqlite3_realloc(void *ptr, int n) { return sqlite3_realloc64(ptr, n); }

void *sqlite3_realloc64(void *ptr, sqlite3_uint64 n) { return realloc(ptr, n); }

// TODO(xx): stub
int sqlite3_config(int i, ...) { return SQLITE_OK; }

int sqlite3_errcode(sqlite3 *db) {
  if (db == nullptr) {
    return SQLITE_MISUSE;
  }
  return db->last_error_.empty() ? SQLITE_OK : SQLITE_ERROR;
}

int sqlite3_extended_errcode(sqlite3 *db) { return sqlite3_errcode(db); }

const char *sqlite3_errmsg(sqlite3 *db) {
  if (db == nullptr) {
    return "";
  }
  return db->last_error_.c_str();
}

void sqlite3_interrupt(sqlite3 *db) {
  // if (db) {
  //   db->con->Interrupt();
  // }
}

const char *sqlite3_libversion(void) {
  // return DuckDB::LibraryVersion();
  static const char *libver = "12345";
  return libver;
}

const char *sqlite3_sourceid(void) {
  static const char *sourceid = "12345";
  return sourceid;

  // return BusTub::SourceID();
}

int sqlite3_reset(sqlite3_stmt *stmt) {
  if (stmt != nullptr) {
    stmt->result_ = nullptr;
    stmt->current_chunk_ = nullptr;
  }
  return SQLITE_OK;
}

// support functions for shell.c
// most are dummies, we don't need them really

int sqlite3_db_status(sqlite3 * /*unused*/, int op, int *pCur, int *pHiwtr, int resetFlg) {
  fprintf(stderr, "sqlite3_db_status: unsupported.\n");
  return -1;
}

// TODO(xx) these should eventually be implemented

int sqlite3_changes(sqlite3 *db) {
  fprintf(stderr, "sqlite3_changes: unsupported.\n");
  return 0;
}

int sqlite3_total_changes(sqlite3 * /*unused*/) {
  fprintf(stderr, "sqlite3_total_changes: unsupported.\n");
  return 0;
}

// checks if input ends with ;
int sqlite3_complete(const char *sql) {
  // FIXME fprintf(stderr, "sqlite3_complete: unsupported.\n");
  return -1;
}

int sqlite3_bind_blob(sqlite3_stmt * /*unused*/, int /*unused*/, const void * /*unused*/, int n,
                      void (* /*unused*/)(void *)) {
  fprintf(stderr, "sqlite3_bind_blob: unsupported.\n");
  return -1;
}

const void *sqlite3_column_blob(sqlite3_stmt * /*unused*/, int iCol) {
  fprintf(stderr, "sqlite3_column_blob: unsupported.\n");
  return nullptr;
}

// length of varchar or blob value
int sqlite3_column_bytes(sqlite3_stmt * /*unused*/, int iCol) {
  fprintf(stderr, "sqlite3_column_bytes: unsupported.\n");
  return -1;
}

sqlite3_value *sqlite3_column_value(sqlite3_stmt * /*unused*/, int iCol) {
  fprintf(stderr, "sqlite3_column_value: unsupported.\n");
  return nullptr;
}

int sqlite3_db_config(sqlite3 * /*unused*/, int op, ...) {
  fprintf(stderr, "sqlite3_db_config: unsupported.\n");
  return -1;
}

int sqlite3_get_autocommit(sqlite3 *db) {
  return 1;
  // TODO(xx) fix this
  // return db->con->context->transaction.IsAutoCommit();
  fprintf(stderr, "sqlite3_get_autocommit: unsupported.\n");
}

int sqlite3_limit(sqlite3 * /*unused*/, int id, int newVal) {
  fprintf(stderr, "sqlite3_limit: unsupported.\n");
  return -1;
}

int sqlite3_stmt_readonly(sqlite3_stmt *pStmt) {
  fprintf(stderr, "sqlite3_stmt_readonly: unsupported.\n");
  return -1;
}

// TODO(xx) pretty easy schema lookup
int sqlite3_table_column_metadata(sqlite3 *db,             /* Connection handle */
                                  const char *zDbName,     /* Database name or NULL */
                                  const char *zTableName,  /* Table name */
                                  const char *zColumnName, /* Column name */
                                  char const **pzDataType, /* OUTPUT: Declared data type */
                                  char const **pzCollSeq,  /* OUTPUT: Collation sequence name */
                                  int *pNotNull,           /* OUTPUT: True if NOT NULL constraint exists */
                                  int *pPrimaryKey,        /* OUTPUT: True if column part of PK */
                                  int *pAutoinc            /* OUTPUT: True if column is auto-increment */
) {
  fprintf(stderr, "sqlite3_table_column_metadata: unsupported.\n");
  return -1;
}

const char *sqlite3_column_decltype(sqlite3_stmt *pStmt, int iCol) {
  // if (!pStmt || !pStmt->prepared) {
  //   return NULL;
  // }
  // auto column_type = pStmt->prepared->types[iCol];
  // switch (column_type.id()) {
  //   case LogicalTypeId::BOOLEAN:
  //     return "BOOLEAN";
  //   case LogicalTypeId::TINYINT:
  //     return "TINYINT";
  //   case LogicalTypeId::SMALLINT:
  //     return "SMALLINT";
  //   case LogicalTypeId::INTEGER:
  //     return "INTEGER";
  //   case LogicalTypeId::BIGINT:
  //     return "BIGINT";
  //   case LogicalTypeId::FLOAT:
  //     return "FLOAT";
  //   case LogicalTypeId::DOUBLE:
  //     return "DOUBLE";
  //   case LogicalTypeId::DECIMAL:
  //     return "DECIMAL";
  //   case LogicalTypeId::DATE:
  //     return "DATE";
  //   case LogicalTypeId::TIME:
  //     return "TIME";
  //   case LogicalTypeId::TIMESTAMP:
  //     return "TIMESTAMP";
  //   case LogicalTypeId::VARCHAR:
  //     return "VARCHAR";
  //   case LogicalTypeId::LIST:
  //     return "LIST";
  //   case LogicalTypeId::STRUCT:
  //     return "STRUCT";
  //   case LogicalTypeId::BLOB:
  //     return "BLOB";
  //   default:
  //     return NULL;
  // }
  return nullptr;
}

int sqlite3_status64(int op, sqlite3_int64 *pCurrent, sqlite3_int64 *pHighwater, int resetFlag) {
  fprintf(stderr, "sqlite3_status64: unsupported.\n");
  return -1;
}

int Sqlite3Status64(sqlite3 * /*unused*/, int op, int *pCur, int *pHiwtr, int resetFlg) {
  fprintf(stderr, "sqlite3_status64: unsupported.\n");
  return -1;
}

int sqlite3_stmt_status(sqlite3_stmt * /*unused*/, int op, int resetFlg) {
  fprintf(stderr, "sqlite3_stmt_status: unsupported.\n");
  return -1;
}

int sqlite3_file_control(sqlite3 * /*unused*/, const char *zDbName, int op, void * /*unused*/) {
  fprintf(stderr, "sqlite3_file_control: unsupported.\n");
  return -1;
}

int sqlite3_declare_vtab(sqlite3 * /*unused*/, const char *zSQL) {
  fprintf(stderr, "sqlite3_declare_vtab: unsupported.\n");
  return -1;
}

const char *sqlite3_vtab_collation(sqlite3_index_info * /*unused*/, int /*unused*/) {
  fprintf(stderr, "sqlite3_vtab_collation: unsupported.\n");
  return nullptr;
}

int sqlite3_sleep(int /*unused*/) {
  fprintf(stderr, "sqlite3_sleep: unsupported.\n");
  return -1;
}

int sqlite3_busy_timeout(sqlite3 * /*unused*/, int ms) {
  fprintf(stderr, "sqlite3_busy_timeout: unsupported.\n");
  return -1;
}

// unlikely to be supported

int sqlite3_trace_v2(sqlite3 * /*unused*/, unsigned uMask, int (*xCallback)(unsigned, void *, void *, void *),
                     void *pCtx) {
  fprintf(stderr, "sqlite3_trace_v2: unsupported.\n");
  return -1;
}

int sqlite3_test_control(int op, ...) {
  fprintf(stderr, "sqlite3_test_control: unsupported.\n");
  return -1;
}

int sqlite3_enable_load_extension(sqlite3 *db, int onoff) {
  // fprintf(stderr, "sqlite3_enable_load_extension: unsupported.\n");
  return -1;
}

int sqlite3_load_extension(sqlite3 *db,       /* Load the extension into this database connection */
                           const char *zFile, /* Name of the shared library containing extension */
                           const char *zProc, /* Entry point.  Derived from zFile if 0 */
                           char **pzErrMsg    /* Put error message here if not 0 */
) {
  // fprintf(stderr, "sqlite3_load_extension: unsupported.\n");
  return -1;
}

int sqlite3_create_module(sqlite3 *db,             /* SQLite connection to register module with */
                          const char *zName,       /* Name of the module */
                          const sqlite3_module *p, /* Methods for the module */
                          void *pClientData        /* Client data for xCreate/xConnect */
) {
  // fprintf(stderr, "sqlite3_create_module: unsupported.\n");
  return -1;
}

int sqlite3_create_function(sqlite3 *db, const char *zFunctionName, int nArg, int eTextRep, void *pApp,
                            void (*xFunc)(sqlite3_context *, int, sqlite3_value **),
                            void (*xStep)(sqlite3_context *, int, sqlite3_value **),
                            void (*xFinal)(sqlite3_context *)) {
  // fprintf(stderr, "sqlite3_create_function: unsupported.\n");
  return -1;
}

int sqlite3_set_authorizer(sqlite3 * /*unused*/, int (*xAuth)(void *, int, const char *, const char *, const char *, const char *),
                           void *pUserData) {
  fprintf(stderr, "sqlite3_set_authorizer: unsupported.\n");
  return -1;
}

// needed in shell timer
static int UnixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow) {
  *piNow = static_cast<sqlite3_int64>(
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
          .count());
  return SQLITE_OK;
}

// virtual file system, providing some dummies to avoid crashes
sqlite3_vfs *sqlite3_vfs_find(const char *zVfsName) {
  // return a dummy because the shell does not check the return code.
  // fprintf(stderr, "sqlite3_vfs_find: unsupported.\n");
  sqlite3_vfs *res = static_cast<sqlite3_vfs *>(sqlite3_malloc(sizeof(sqlite3_vfs)));
  res->xCurrentTimeInt64 = UnixCurrentTimeInt64;
  res->iVersion = 2;
  res->zName = "dummy";
  res->pNext = nullptr;
  assert(res);
  return res;
}
int sqlite3_vfs_register(sqlite3_vfs * /*unused*/, int makeDflt) {
  // fprintf(stderr, "sqlite3_vfs_register: unsupported.\n");
  return -1;
}

// backups, unused

int sqlite3_backup_step(sqlite3_backup *p, int nPage) {
  fprintf(stderr, "sqlite3_backup_step: unsupported.\n");
  return -1;
}
int sqlite3_backup_finish(sqlite3_backup *p) {
  fprintf(stderr, "sqlite3_backup_finish: unsupported.\n");
  return -1;
}

sqlite3_backup *sqlite3_backup_init(sqlite3 *pDest,         /* Destination database handle */
                                    const char *zDestName,  /* Destination database name */
                                    sqlite3 *pSource,       /* Source database handle */
                                    const char *zSourceName /* Source database name */
) {
  fprintf(stderr, "sqlite3_backup_init: unsupported.\n");
  return nullptr;
}

// UDF support stuff, unused for now. These cannot be called as create_function above is disabled

SQLITE_API sqlite3 *sqlite3_context_db_handle(sqlite3_context * /*unused*/) { return nullptr; }

void *sqlite3_user_data(sqlite3_context * /*unused*/) { return nullptr; }

#ifdef _WIN32
#include <windows.h>

static void *sqlite3MallocZero(size_t n) {
  auto res = sqlite3_malloc(n);
  assert(res);
  memset(res, 0, n);
  return res;
}

static LPWSTR winUtf8ToUnicode(const char *zText) {
  int nChar;
  LPWSTR zWideText;

  nChar = MultiByteToWideChar(CP_UTF8, 0, zText, -1, NULL, 0);
  if (nChar == 0) {
    return 0;
  }
  zWideText = (LPWSTR)sqlite3MallocZero(nChar * sizeof(WCHAR));
  if (zWideText == 0) {
    return 0;
  }
  nChar = MultiByteToWideChar(CP_UTF8, 0, zText, -1, zWideText, nChar);
  if (nChar == 0) {
    sqlite3_free(zWideText);
    zWideText = 0;
  }
  return zWideText;
}

static char *winUnicodeToMbcs(LPCWSTR zWideText, int useAnsi) {
  int nByte;
  char *zText;
  int codepage = useAnsi ? CP_ACP : CP_OEMCP;

  nByte = WideCharToMultiByte(codepage, 0, zWideText, -1, 0, 0, 0, 0);
  if (nByte == 0) {
    return 0;
  }
  zText = (char *)sqlite3MallocZero(nByte);
  if (zText == 0) {
    return 0;
  }
  nByte = WideCharToMultiByte(codepage, 0, zWideText, -1, zText, nByte, 0, 0);
  if (nByte == 0) {
    sqlite3_free(zText);
    zText = 0;
  }
  return zText;
}

static char *winUtf8ToMbcs(const char *zText, int useAnsi) {
  char *zTextMbcs;
  LPWSTR zTmpWide;

  zTmpWide = winUtf8ToUnicode(zText);
  if (zTmpWide == 0) {
    return 0;
  }
  zTextMbcs = winUnicodeToMbcs(zTmpWide, useAnsi);
  sqlite3_free(zTmpWide);
  return zTextMbcs;
}

SQLITE_API char *sqlite3_win32_utf8_to_mbcs_v2(const char *zText, int useAnsi) { return winUtf8ToMbcs(zText, useAnsi); }

LPWSTR sqlite3_win32_utf8_to_unicode(const char *zText) { return winUtf8ToUnicode(zText); }

static LPWSTR winMbcsToUnicode(const char *zText, int useAnsi) {
  int nByte;
  LPWSTR zMbcsText;
  int codepage = useAnsi ? CP_ACP : CP_OEMCP;

  nByte = MultiByteToWideChar(codepage, 0, zText, -1, NULL, 0) * sizeof(WCHAR);
  if (nByte == 0) {
    return 0;
  }
  zMbcsText = (LPWSTR)sqlite3MallocZero(nByte * sizeof(WCHAR));
  if (zMbcsText == 0) {
    return 0;
  }
  nByte = MultiByteToWideChar(codepage, 0, zText, -1, zMbcsText, nByte);
  if (nByte == 0) {
    sqlite3_free(zMbcsText);
    zMbcsText = 0;
  }
  return zMbcsText;
}

static char *winUnicodeToUtf8(LPCWSTR zWideText) {
  int nByte;
  char *zText;

  nByte = WideCharToMultiByte(CP_UTF8, 0, zWideText, -1, 0, 0, 0, 0);
  if (nByte == 0) {
    return 0;
  }
  zText = (char *)sqlite3MallocZero(nByte);
  if (zText == 0) {
    return 0;
  }
  nByte = WideCharToMultiByte(CP_UTF8, 0, zWideText, -1, zText, nByte, 0, 0);
  if (nByte == 0) {
    sqlite3_free(zText);
    zText = 0;
  }
  return zText;
}

static char *winMbcsToUtf8(const char *zText, int useAnsi) {
  char *zTextUtf8;
  LPWSTR zTmpWide;

  zTmpWide = winMbcsToUnicode(zText, useAnsi);
  if (zTmpWide == 0) {
    return 0;
  }
  zTextUtf8 = winUnicodeToUtf8(zTmpWide);
  sqlite3_free(zTmpWide);
  return zTextUtf8;
}

SQLITE_API char *sqlite3_win32_mbcs_to_utf8_v2(const char *zText, int useAnsi) { return winMbcsToUtf8(zText, useAnsi); }

SQLITE_API char *sqlite3_win32_unicode_to_utf8(LPCWSTR zWideText) { return winUnicodeToUtf8(zWideText); }

#endif

// TODO(xx) complain
SQLITE_API void sqlite3_result_blob(sqlite3_context * /*unused*/, const void * /*unused*/, int /*unused*/,
                                    void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_blob64(sqlite3_context * /*unused*/, const void * /*unused*/, sqlite3_uint64 /*unused*/,
                                      void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_double(sqlite3_context * /*unused*/, double /*unused*/) {}
SQLITE_API void sqlite3_result_error(sqlite3_context * /*unused*/, const char * /*unused*/, int /*unused*/) {}
SQLITE_API void sqlite3_result_error16(sqlite3_context * /*unused*/, const void * /*unused*/, int /*unused*/) {}
SQLITE_API void sqlite3_result_error_toobig(sqlite3_context * /*unused*/) {}
SQLITE_API void sqlite3_result_error_nomem(sqlite3_context * /*unused*/) {}
SQLITE_API void sqlite3_result_error_code(sqlite3_context * /*unused*/, int /*unused*/) {}
SQLITE_API void sqlite3_result_int(sqlite3_context * /*unused*/, int /*unused*/) {}
SQLITE_API void sqlite3_result_int64(sqlite3_context * /*unused*/, sqlite3_int64 /*unused*/) {}
SQLITE_API void sqlite3_result_null(sqlite3_context * /*unused*/) {}
SQLITE_API void sqlite3_result_text(sqlite3_context * /*unused*/, const char * /*unused*/, int /*unused*/,
                                    void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_text64(sqlite3_context * /*unused*/, const char * /*unused*/, sqlite3_uint64 /*unused*/,
                                      void (* /*unused*/)(void *), unsigned char encoding) {}
SQLITE_API void sqlite3_result_text16(sqlite3_context * /*unused*/, const void * /*unused*/, int /*unused*/,
                                      void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_text16le(sqlite3_context * /*unused*/, const void * /*unused*/, int /*unused*/,
                                        void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_text16be(sqlite3_context * /*unused*/, const void * /*unused*/, int /*unused*/,
                                        void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_value(sqlite3_context * /*unused*/, sqlite3_value * /*unused*/) {}
SQLITE_API void sqlite3_result_pointer(sqlite3_context * /*unused*/, void * /*unused*/, const char * /*unused*/,
                                       void (* /*unused*/)(void *)) {}
SQLITE_API void sqlite3_result_zeroblob(sqlite3_context * /*unused*/, int n) {}
SQLITE_API int sqlite3_result_zeroblob64(sqlite3_context * /*unused*/, sqlite3_uint64 n) { return -1; }

// TODO(xx) complain
const void *sqlite3_value_blob(sqlite3_value * /*unused*/) { return nullptr; }
double sqlite3_value_double(sqlite3_value * /*unused*/) { return 0; }
int sqlite3_value_int(sqlite3_value * /*unused*/) { return 0; }
sqlite3_int64 sqlite3_value_int64(sqlite3_value * /*unused*/) { return 0; }
void *sqlite3_value_pointer(sqlite3_value * /*unused*/, const char * /*unused*/) { return nullptr; }
const unsigned char *sqlite3_value_text(sqlite3_value * /*unused*/) { return nullptr; }
SQLITE_API const void *sqlite3_value_text16(sqlite3_value * /*unused*/) { return nullptr; }
SQLITE_API const void *sqlite3_value_text16le(sqlite3_value * /*unused*/) { return nullptr; }
SQLITE_API const void *sqlite3_value_text16be(sqlite3_value * /*unused*/) { return nullptr; }
SQLITE_API int sqlite3_value_bytes(sqlite3_value * /*unused*/) { return 0; }
SQLITE_API int sqlite3_value_bytes16(sqlite3_value * /*unused*/) { return 0; }
SQLITE_API int sqlite3_value_type(sqlite3_value * /*unused*/) { return 0; }
SQLITE_API int sqlite3_value_numeric_type(sqlite3_value * /*unused*/) { return 0; }
SQLITE_API int sqlite3_value_nochange(sqlite3_value * /*unused*/) { return 0; }

SQLITE_API void *sqlite3_aggregate_context(sqlite3_context * /*unused*/, int nBytes) {
  fprintf(stderr, "sqlite3_aggregate_context: unsupported.\n");

  return nullptr;
}

SQLITE_API int sqlite3_create_collation(sqlite3 * /*unused*/, const char *zName, int eTextRep, void *pArg,
                                        int (*xCompare)(void *, int, const void *, int, const void *)) {
  return SQLITE_ERROR;
}

SQLITE_API int sqlite3_create_window_function(sqlite3 *db, const char *zFunctionName, int nArg, int eTextRep,
                                              void *pApp, void (*xStep)(sqlite3_context *, int, sqlite3_value **),
                                              void (*xFinal)(sqlite3_context *), void (*xValue)(sqlite3_context *),
                                              void (*xInverse)(sqlite3_context *, int, sqlite3_value **),
                                              void (*xDestroy)(void *)) {
  fprintf(stderr, "sqlite3_create_window_function: unsupported.\n");
  return SQLITE_ERROR;
}

SQLITE_API sqlite3 *sqlite3_db_handle(sqlite3_stmt *s) { return s->db_; }

SQLITE_API char *sqlite3_expanded_sql(sqlite3_stmt *pStmt) {
  fprintf(stderr, "sqlite3_expanded_sql: unsupported.\n");
  return nullptr;
}

SQLITE_API int sqlite3_keyword_check(const char *str, int len) {
  return static_cast<int>(bustub::Parser::IsKeyword(std::string(str, len)));
}

SQLITE_API int sqlite3_keyword_count(void) {
  fprintf(stderr, "sqlite3_keyword_count: unsupported.\n");
  return 0;
}

SQLITE_API int sqlite3_keyword_name(int /*unused*/, const char ** /*unused*/, int * /*unused*/) {
  fprintf(stderr, "sqlite3_keyword_name: unsupported.\n");
  return 0;
}

SQLITE_API void sqlite3_progress_handler(sqlite3 * /*unused*/, int /*unused*/, int (* /*unused*/)(void *),
                                         void * /*unused*/) {
  fprintf(stderr, "sqlite3_progress_handler: unsupported.\n");
}

SQLITE_API int sqlite3_stmt_isexplain(sqlite3_stmt *pStmt) {
  // if (!pStmt || !pStmt->prepared) {
  //   return 0;
  // }
  // return pStmt->prepared->type == StatementType::EXPLAIN_STATEMENT;
  return 0;
}

SQLITE_API int sqlite3_vtab_config(sqlite3 * /*unused*/, int op, ...) {
  fprintf(stderr, "sqlite3_vtab_config: unsupported.\n");
  return SQLITE_ERROR;
}
