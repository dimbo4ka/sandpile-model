add_library(sandpile sand_pile.cpp sand_pile.h)
target_include_directories(sandpile PUBLIC ${CMAKE_SOURCE_DIR})