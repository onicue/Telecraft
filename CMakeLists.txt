cmake_minimum_required(VERSION 3.20)
project(Telecraft
        VERSION 0.0.1
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

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

if(USE_BOOST OR ENABLE_TESTS)
  find_package(Boost ${Boost_MIN_VERSION} REQUIRED COMPONENTS system filesystem unit_test_framework)
  if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} INTERFACE Boost::system Boost::filesystem Boost::unit_test_framework)
  else()
    message(FATAL_ERROR "Boost not found!")
  endif()
endif()

include(FetchContent)

set(GLAZE_VERSION "main")
FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG ${GLAZE_VERSION}
  GIT_SHALLOW TRUE
  FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(glaze)

if(ENABLE_TESTS)
    if(NOT USE_BOOST)
        message(FATAL_ERROR "Boost is required for tests but USE_BOOST is OFF")
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
include(CMakePackageConfigHelpers)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/SI DESTINATION include)

install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)
