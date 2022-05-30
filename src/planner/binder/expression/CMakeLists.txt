add_library(
  bustub_bind_expression
  OBJECT
  bind_columnref_expression.cpp
  bind_constant_expression.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_bind_expression>
    PARENT_SCOPE)
