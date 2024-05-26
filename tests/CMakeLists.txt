# create the testing file and list of tests

# add_executable(TestInstantiator main.cpp)
# target_link_libraries(TestInstantiator
#                       PRIVATE
#                       # ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#                       Boost::filesystem
#                       Boost::system
#                       Boost::unit_test_framework
# )
#
# add_test(hui TestInstantiator)




# Create the list of tests
set(TestToRun
  main.cpp
  # Add more tests as needed
)

set(TestLibs
    Boost::filesystem
    Boost::system
    Boost::unit_test_framework
)

function(add_new_test test_name files libraries command)
  add_executable(${test_name} ${files})
  target_link_libraries(${test_name} ${libraries})
  add_test(NAME ${test_name} COMMAND ${test_name} ${command})
endfunction()

foreach(test ${TestToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_new_test(${TName} ${test} "${TestLibs}" "")
endforeach()
