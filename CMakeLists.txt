cmake_minimum_required(VERSION 3.26)
project(Telecraft
        DESCRIPTION "A versatile and efficient Telegram message builder library"
        HOMEPAGE_URL "https://github.com/onicue/Telecraft"
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(Boost_MIN_VERSION "1.75.0")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

option(USE_BOOST "Use Boost libraries" ON)
option(ENABLE_TESTS "Enable testing" ON)

if(USE_BOOST OR ENABLE_TESTS)
  find_package(Boost ${Boost_MIN_VERSION} REQUIRED COMPONENTS system filesystem unit_test_framework)
  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
  else()
    message(FATAL_ERROR "Boost not found!")
  endif()
endif()

if(ENABLE_TESTS)
    if(NOT USE_BOOST)
        message(FATAL_ERROR "Boost is required for tests but USE_BOOST is OFF")
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()
