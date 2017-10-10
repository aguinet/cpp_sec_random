add_executable(test_shuffle shuffle.cpp)
target_link_libraries(test_shuffle sec_random)
add_test(shuffle test_shuffle)

add_executable(test_rand_int rand_int.cpp)
target_link_libraries(test_rand_int sec_random)
add_test(rand_int test_rand_int)
