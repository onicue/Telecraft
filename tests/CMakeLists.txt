set(TestToRun
  main.cpp
  ParamsManagement.cpp
  MethodBuilders.cpp
  # Add more tests as needed
)

set(TestLibs #libraries for TestToRun
    Boost::filesystem
    Boost::system
    Boost::unit_test_framework
    glaze::glaze
)

function(add_new_test test_name files libraries command)
  add_executable(${test_name} ${files})
  target_link_libraries(${test_name} ${libraries})
  add_test(NAME ${test_name} COMMAND ${test_name} ${command})
  target_include_directories(${test_name} PRIVATE ../include)
endfunction()

foreach(test ${TestToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_new_test(${TName} ${test} "${TestLibs}" "")
endforeach()

#if you need specific tests add at the bottom
