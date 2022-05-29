add_library(
  bustub_common_util
  string_util.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_common_util>
    PARENT_SCOPE)
