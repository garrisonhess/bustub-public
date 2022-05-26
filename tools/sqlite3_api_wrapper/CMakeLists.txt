include_directories(include)
add_subdirectory(sqlite3)

add_definitions(-DSQLITE_SHELL_IS_UTF8)

include_directories(../../third_party/utf8proc/include)

add_library(sqlite3_api_wrapper_static STATIC sqlite3_api_wrapper.cpp
                                              ${ALL_OBJECT_FILES})
target_link_libraries(sqlite3_api_wrapper_static bustub_static bustub_utf8proc)

add_library(sqlite3_api_wrapper SHARED sqlite3_api_wrapper.cpp
                                        ${ALL_OBJECT_FILES})
target_link_libraries(sqlite3_api_wrapper duckdb ${DUCKDB_EXTRA_LINK_FLAGS})

include_directories(../../third_party/catch)

include_directories(test/include)
add_subdirectory(test)

add_executable(test_sqlite3_api_wrapper ${SQLITE_TEST_FILES})
target_link_libraries(test_sqlite3_api_wrapper sqlite3_api_wrapper)
