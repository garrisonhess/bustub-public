add_subdirectory(enums)
add_library(
  bustub_common
  OBJECT
  field_writer.cpp
  printer.cpp
  serializer.cpp
  string_util.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_common>
    PARENT_SCOPE)
