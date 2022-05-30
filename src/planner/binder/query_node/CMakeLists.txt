add_library(
  bustub_bind_query_node
  OBJECT
  bind_select_node.cpp
  plan_query_node.cpp
  plan_select_node.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_bind_query_node>
    PARENT_SCOPE)
