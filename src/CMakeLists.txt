add_subdirectory(buffer)
add_subdirectory(catalog)
add_subdirectory(common)
add_subdirectory(concurrency)
add_subdirectory(container)
add_subdirectory(execution)
add_subdirectory(recovery)
add_subdirectory(storage)
add_subdirectory(type)

add_library(bustub SHARED ${ALL_OBJECT_FILES})
target_link_libraries(bustub bustub_murmur3)

target_include_directories(
bustub PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# install(FILES "${PROJECT_SOURCE_DIR}/src/include/duckdb.hpp"
#     "${PROJECT_SOURCE_DIR}/src/include/duckdb.h"
# DESTINATION "${INSTALL_INCLUDE_DIR}")
