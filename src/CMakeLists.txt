add_subdirectory(buffer)
add_subdirectory(catalog)
add_subdirectory(common)
add_subdirectory(concurrency)
add_subdirectory(container)
add_subdirectory(execution)
add_subdirectory(recovery)
add_subdirectory(storage)
add_subdirectory(type)

file(GLOB_RECURSE bustub_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*/*.cpp)
add_library(bustub SHARED ${bustub_sources})
# add_library(bustub SHARED ${ALL_OBJECT_FILES})

# murmur3
file(GLOB_RECURSE murmur3_sources
        ${PROJECT_SOURCE_DIR}/third_party/murmur3/*.cpp ${PROJECT_SOURCE_DIR}/third_party/murmur3/*.h)
add_library(bustub_murmur3 SHARED ${murmur3_sources})
target_link_libraries(bustub bustub_murmur3)

target_include_directories(
bustub PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# install(FILES "${PROJECT_SOURCE_DIR}/src/include/duckdb.hpp"
#     "${PROJECT_SOURCE_DIR}/src/include/duckdb.h"
# DESTINATION "${INSTALL_INCLUDE_DIR}")
