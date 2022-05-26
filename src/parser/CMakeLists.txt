include_directories(../../third_party/libpg_query/include)
include_directories(../../third_party/libpg_query)

add_library(bustub_parser parser.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_parser>
    PARENT_SCOPE)
