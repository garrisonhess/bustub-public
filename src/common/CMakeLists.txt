add_subdirectory(util)

add_library(
  bustub_common
  OBJECT
  config.cpp
  field_writer.cpp
  printer.cpp
  serializer.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_common>
    PARENT_SCOPE)
