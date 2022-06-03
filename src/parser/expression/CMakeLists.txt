add_library(
  bustub_expression
  OBJECT
  constant_expression.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_expression>
    PARENT_SCOPE)
