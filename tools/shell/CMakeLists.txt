add_definitions(-DSQLITE_OMIT_LOAD_EXTENSION=1)

set(SHELL_SOURCES shell.c)
add_definitions(-DHAVE_LINENOISE=1)
set(SHELL_SOURCES ${SHELL_SOURCES} linenoise.cpp)
include_directories(../../third_party/utf8proc/include)

option(STATIC_LIBCPP "Statically link CLI to libc++" FALSE)

include_directories(include)
include_directories(../sqlite3_api_wrapper/include)
add_executable(shell ${SHELL_SOURCES})
target_link_libraries(shell sqlite3_api_wrapper_static
                      ${DUCKDB_EXTRA_LINK_FLAGS})
# link_threads(shell)
if(STATIC_LIBCPP)
  message("Statically linking CLI")
  target_link_libraries(shell -static-libstdc++ -static-libgcc)
endif()

target_link_libraries(shell duckdb_utf8proc)

set_target_properties(shell PROPERTIES OUTPUT_NAME duckdb)
set_target_properties(shell PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                       ${CMAKE_BINARY_DIR})

install(TARGETS shell RUNTIME DESTINATION "${INSTALL_BIN_DIR}")
