# this directory contains some of the original code from SQLite mainly helper
# functions that we don't really care about replacing ourselves

set(SQLITE_FILES printf.c strglob.c)

add_library(sqlite3_api_wrapper_sqlite3 OBJECT ${SQLITE_FILES})
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:sqlite3_api_wrapper_sqlite3>
    PARENT_SCOPE)
