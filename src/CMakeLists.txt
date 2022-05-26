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

target_link_libraries(bustub bustub_buffer bustub_catalog bustub_common_util bustub_common bustub_concurrency bustub_execution bustub_recovery bustub_type bustub_murmur3)

target_include_directories(
bustub PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
