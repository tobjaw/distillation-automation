cmake_minimum_required(VERSION 3.9.5)
project (distillation-automation)

include_directories("${PROJECT_SOURCE_DIR}/src")
add_executable(distillation-automation src/main.c)
