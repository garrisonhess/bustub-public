add_library(
  bustub_planner
  OBJECT
  plan_factory.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_planner>
    PARENT_SCOPE)
