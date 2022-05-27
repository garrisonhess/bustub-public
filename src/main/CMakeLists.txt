# add_subdirectory(relation)
add_library(bustub_main
            OBJECT
            client_context.cpp
            config.cpp
            connection.cpp
            connection_manager.cpp
            database.cpp
            query_result.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_main>
    PARENT_SCOPE)
