project(simple)
project(pure)
project(exceptions)
project(locks)

find_package(Boost 1.81 REQUIRED COMPONENTS unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(../src)

add_executable(simple simple.cpp)
add_executable(pure pure.cpp)
add_executable(exceptions exceptions.cpp)
add_executable(locks locks.cpp)

target_link_libraries(simple PUBLIC Boost::unit_test_framework pthread)
target_link_libraries(pure PUBLIC Boost::unit_test_framework pthread)
target_link_libraries(exceptions PUBLIC Boost::unit_test_framework pthread)
target_link_libraries(locks PUBLIC Boost::unit_test_framework pthread)

enable_testing()
add_test(
        NAME simple
        COMMAND simple
)
###
#
# this test show possible data race
#
#add_test(
#        NAME pure
#        COMMAND pure
#)
###
add_test(
        NAME exceptions
        COMMAND exceptions
)
add_test(
        NAME locks
        COMMAND locks
)