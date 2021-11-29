cmake_minimum_required(VERSION 3.0.0)
project(tcp_client VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(tcp_client main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(tcp_client PRIVATE Threads::Threads)