include_directories(include)
include_directories(../../third_party/libpg_query/include)
include_directories(../sqlite3_api_wrapper/include)
add_executable(shell shell.c linenoise.cpp)

target_link_libraries(shell sqlite3_api_wrapper bustub_utf8proc ${BUSTUB_EXTRA_LINK_FLAGS})
target_link_libraries(shell pthread)

set_target_properties(shell PROPERTIES OUTPUT_NAME bustub)
set_target_properties(shell PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

install(TARGETS shell RUNTIME DESTINATION "${INSTALL_BIN_DIR}")
