add_library(iterations iterations.cpp iterations.h)
target_include_directories(iterations PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(iterations PRIVATE parser bitmap sandpile)