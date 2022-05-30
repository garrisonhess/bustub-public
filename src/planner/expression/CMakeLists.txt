add_library(
  bustub_planner_expression
  OBJECT
  bound_columnref_expression.cpp
  bound_constant_expression.cpp
  bound_operator_expression.cpp
  bound_reference_expression.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_planner_expression>
    PARENT_SCOPE)
