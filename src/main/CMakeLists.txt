include_directories(../../third_party/libpg_query/include)
include_directories(../../third_party/libpg_query)

add_library(bustub_main
            OBJECT
            client_context.cpp
            config.cpp
            connection_manager.cpp
            connection.cpp
            database.cpp
            prepared_statement.cpp
            query_result.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bustub_main>
    PARENT_SCOPE)
