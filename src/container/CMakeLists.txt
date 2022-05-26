add_library(
  bustub_container
  extendible_hash_table.cpp
  linear_probe_hash_table.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_container>
    PARENT_SCOPE)
