add_library(
  bustub_expression
  OBJECT
  between_expression.cpp
  case_expression.cpp
  cast_expression.cpp
  conjunction_expression.cpp
  columnref_expression.cpp
  comparison_expression.cpp
  constant_expression.cpp
  default_expression.cpp
  operator_expression.cpp
  star_expression.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_expression>
    PARENT_SCOPE)
