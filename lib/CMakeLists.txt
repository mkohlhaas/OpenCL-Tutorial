cmake_minimum_required(VERSION 3.27)

project(errorHandling LANGUAGES C)

add_library(${PROJECT_NAME} STATIC err_code.c wtime.c device_info.c)
