set(SOURCES main.cpp)

add_executable(test ${SOURCES})

target_include_directories(test PRIVATE ../include)
