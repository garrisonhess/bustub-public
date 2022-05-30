add_library(
  bustub_transformer_statement
  OBJECT
  transform_create_index.cpp
  transform_create_table.cpp
  transform_delete.cpp
  transform_insert.cpp
  transform_select.cpp
  transform_select_node.cpp
  transform_update.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_transformer_statement>
    PARENT_SCOPE)
