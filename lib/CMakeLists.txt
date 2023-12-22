cmake_minimum_required(VERSION 3.27)

project(ErrorHandling LANGUAGES C)

add_library(${PROJECT_NAME} STATIC err_code.c)
