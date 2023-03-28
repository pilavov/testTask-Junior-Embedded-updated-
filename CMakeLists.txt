cmake_minimum_required(VERSION 3.24)
project(TestTask_OM_T3_Junior_Embedded)

set(CMAKE_CXX_STANDARD 14)

add_executable(TestTask_OM_T3_Junior_Embedded main.cpp DataBase.cpp DataBase.h)
target_link_libraries(TestTask_OM_T3_Junior_Embedded LINK_PUBLIC sqlite3)