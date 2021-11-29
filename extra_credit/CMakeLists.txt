cmake_minimum_required(VERSION 3.0.0)
project(extra_credit VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(extra_credit src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(extra_credit PRIVATE Threads::Threads)