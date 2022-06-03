include_directories(../../third_party/libpg_query/include)
include_directories(../../third_party/libpg_query)

# add_subdirectory(expression)
# add_subdirectory(statement)
# add_subdirectory(tableref)
add_subdirectory(transform)

add_library(
  bustub_parser
  OBJECT
  keyword_helper.cpp
  parser.cpp
  transformer.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_parser>
    PARENT_SCOPE)
