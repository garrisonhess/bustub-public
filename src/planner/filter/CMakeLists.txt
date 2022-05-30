add_library(bustub_planner_filter OBJECT conjunction_filter.cpp
                  constant_filter.cpp null_filter.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_planner_filter>
    PARENT_SCOPE)
