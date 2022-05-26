if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

include_directories(include)

add_library(bustub_sqlite3 sqlite3.c)
target_link_libraries(bustub_sqlite3 pthread)

