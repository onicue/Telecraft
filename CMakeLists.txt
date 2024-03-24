cmake_minimum_required(VERSION 3.26)
project(Telecraft
        DESCRIPTION "A versatile and efficient Telegram message builder library"
        HOMEPAGE_URL "https://github.com/onicue/Telecraft"
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(test)
