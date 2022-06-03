add_library(
  bustub_transformer_helpers
  OBJECT
  transform_typename.cpp
  nodetype_to_string.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_transformer_helpers>
    PARENT_SCOPE)
