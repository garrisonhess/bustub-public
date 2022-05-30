add_library(
  bustub_bind_tableref
  OBJECT
  bind_basetableref.cpp
  bind_emptytableref.cpp
  bind_expressionlistref.cpp
  bind_joinref.cpp
  plan_basetableref.cpp
  plan_dummytableref.cpp
  plan_expressionlistref.cpp
  plan_joinref.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_bind_tableref>
    PARENT_SCOPE)
