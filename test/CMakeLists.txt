# Some compilers warn that the return value of cr_assert gets converted from an
# int to a unsigned char:1. To avoid this error in those compilers, turn off
# conversion warnings for test code.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-conversion")

list(APPEND CMAKE_CTEST_ARGUMENTS --verbose)

# If you need to test a library called foo, use lines like the following.
# add_executable(test_foo test_foo.c)
# target_link_libraries(test_foo
# PRIVATE foo
# PUBLIC ${CRITERION}
# )
# add_test(
# NAME test_foo
# COMMAND test_foo ${CRITERION_FLAGS}
# )
add_executable(test_view test_view.c)
target_link_libraries(test_view
    PRIVATE view
    PUBLIC ${CRITERION}
)
add_test(
    NAME test_view
    COMMAND test_view ${CRITERION_FLAGS}
)

# add_executable(test_model test_model.c)
# target_link_libraries(test_model
# PRIVATE model
# PUBLIC ${CRITERION}
# )
# add_test(
# NAME test_model
# COMMAND test_model ${CRITERION_FLAGS}
# )
# add_executable(test_utils test_utils.c)
# target_link_libraries(test_utils
# PRIVATE utils
# PUBLIC ${CRITERION}
# )
# add_test(
# NAME test_utils
# COMMAND test_utils ${CRITERION_FLAGS}
# )

# add_executable(test_server test_server.c)
# target_link_libraries(test_server
# PRIVATE server
# PUBLIC ${CRITERION}
# )
# add_test(
# NAME test_server
# COMMAND test_server ${CRITERION_FLAGS}
# )
