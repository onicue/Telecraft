set(SOURCES main.cpp net/posix_ssl.cpp)

add_executable(test ${SOURCES})
# target_compile_options(test PRIVATE -fsanitize=address)
# target_link_options(test PRIVATE -fsanitize=address)

target_compile_definitions(test PRIVATE -DUSE_SSL)
target_link_libraries(test PRIVATE ssl crypto)

target_include_directories(test PRIVATE ../include)
