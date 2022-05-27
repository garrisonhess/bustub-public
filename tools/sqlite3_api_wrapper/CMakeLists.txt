include_directories(include)
add_subdirectory(sqlite3)

include_directories(../../third_party/utf8proc/include)

add_library(sqlite3_api_wrapper SHARED sqlite3_api_wrapper.cpp
                                    ${ALL_OBJECT_FILES})
# target_link_libraries(sqlite3_api_wrapper bustub ${DUCKDB_EXTRA_LINK_FLAGS})
target_link_libraries(sqlite3_api_wrapper bustub)
# link_threads(sqlite3_api_wrapper)

# link_extension_libraries(sqlite3_api_wrapper)

include_directories(../../third_party/catch)

# add_executable(test_sqlite3_api_wrapper test_sqlite3_api_wrapper.cpp)
# target_link_libraries(test_sqlite3_api_wrapper sqlite3_api_wrapper)
