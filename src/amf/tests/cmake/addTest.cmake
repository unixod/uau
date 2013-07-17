#
# addTest(test_name file_stem)
#
# Parameters:
# test_name     -   test description string
# file_stem     -   test-file name stem string
# <<rest_args>> -   all rest, optional arguments are considered as
#                   library names or flags for target_link_libraries invokation.
#
# Description:
# Compailes the test_${file_stem}.cpp to binary ${file_stem}.test
# and add this binary as test.
#
function(addTest test_name file_stem)
    set(CPP_SOURCE_FILE_NAME "test_${file_stem}.cpp")
    set(TEST_FILE_NAME "${file_stem}.test")
    add_executable(${TEST_FILE_NAME} ${CPP_SOURCE_FILE_NAME})
    target_link_libraries(${TEST_FILE_NAME} ${ARGN})
    add_test(${test_name} ${TEST_FILE_NAME})
endfunction()
