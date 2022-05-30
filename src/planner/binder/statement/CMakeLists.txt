add_library(
  bustub_bind_statement
  OBJECT
  bind_create.cpp
  bind_create_table.cpp
  bind_delete.cpp
  bind_insert.cpp
  bind_select.cpp
  bind_update.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_bind_statement>
    PARENT_SCOPE)
