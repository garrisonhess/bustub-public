if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

include_directories(include)

add_library(bustub_utf8proc SHARED utf8proc.cpp utf8proc_wrapper.cpp)
