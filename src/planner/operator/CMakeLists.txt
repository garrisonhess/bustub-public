add_library(
  bustub_planner_operator
  OBJECT
  logical_empty_result.cpp
  logical_filter.cpp
  logical_limit.cpp
  logical_join.cpp
  logical_projection.cpp
  logical_distinct.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_planner_operator>
    PARENT_SCOPE)
