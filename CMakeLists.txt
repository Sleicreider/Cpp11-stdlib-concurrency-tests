cmake_minimum_required(VERSION 3.0.0)

project(InTest1)

file(GLOB_RECURSE SOURCE Classes/*.cpp)
file(GLOB_RECURSE HEADER Classss/*.h)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME} PUBLIC Classes)
