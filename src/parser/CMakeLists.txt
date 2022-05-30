include_directories(../../third_party/libpg_query/include)
include_directories(../../third_party/libpg_query)

add_subdirectory(expression)
add_subdirectory(query_node)
add_subdirectory(statement)
add_subdirectory(tableref)
add_subdirectory(transform)

add_library(
  bustub_parser
  OBJECT
  base_expression.cpp
  column_definition.cpp
  expression_util.cpp
  keyword_helper.cpp
  parsed_expression.cpp
  parser.cpp
  query_error_context.cpp
  query_node.cpp
  result_modifier.cpp
  tableref.cpp
  transformer.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_parser>
    PARENT_SCOPE)
