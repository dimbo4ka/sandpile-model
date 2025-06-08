add_library(bitmap bitmap.cpp bitmap.h)
target_include_directories(bitmap PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(bitmap PRIVATE sand_pile)