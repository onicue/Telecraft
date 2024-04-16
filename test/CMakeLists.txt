set(SOURCES main.cpp)

add_executable(test ${SOURCES})
# target_compile_options(test PRIVATE -fsanitize=address)
# target_link_options(test PRIVATE -fsanitize=address)

target_include_directories(test PRIVATE ../include)
