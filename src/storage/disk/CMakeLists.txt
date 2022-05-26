add_library(bustub_storage_disk disk_manager.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_storage_disk>
    PARENT_SCOPE)
