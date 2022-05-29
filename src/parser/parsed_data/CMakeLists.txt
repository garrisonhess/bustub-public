add_library(bustub_parsed_data OBJECT alter_table_info.cpp
                  sample_options.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_parsed_data>
    PARENT_SCOPE)
