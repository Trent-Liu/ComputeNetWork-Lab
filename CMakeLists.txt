cmake_minimum_required(VERSION 3.20)
project(SeverTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(SeverTest webServer.cpp task.cpp task.h thread_pool.h locker.h sockettask.cpp sockettask.h)

find_package(Threads REQUIRED)
target_link_libraries(SeverTest Threads::Threads)