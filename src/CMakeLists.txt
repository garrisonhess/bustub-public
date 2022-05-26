add_subdirectory(buffer)
add_subdirectory(catalog)
add_subdirectory(common)
add_subdirectory(concurrency)
add_subdirectory(container)
add_subdirectory(execution)
add_subdirectory(parser)
add_subdirectory(recovery)
add_subdirectory(storage)
add_subdirectory(type)

add_library(bustub SHARED ${ALL_OBJECT_FILES})
target_link_libraries(bustub bustub_murmur3 bustub_pg_query bustub_sqlite3)

add_library(bustub_static STATIC ${ALL_OBJECT_FILES})
target_link_libraries(bustub_static bustub_murmur3 bustub_pg_query bustub_sqlite3)

target_include_directories(
bustub PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_include_directories(
bustub_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
DIRECTORY "${PROJECT_SOURCE_DIR}/src/include/bustub"
DESTINATION "${INSTALL_INCLUDE_DIR}"
FILES_MATCHING
PATTERN "*.h")
install(FILES "${PROJECT_SOURCE_DIR}/src/include/bustub.h"
        DESTINATION "${INSTALL_INCLUDE_DIR}")

install(
    TARGETS bustub bustub_static
    EXPORT "${BUSTUB_EXPORT_SET}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}")
